// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilComponent.h"
#include "CRRecoilInterface.h"
#include "CRRecoilPattern.h"


UCRRecoilComponent::UCRRecoilComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}


void UCRRecoilComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCRRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!RecoilPattern)
	{
		return;
	}
	if(!GetTargetController())
	{
		return;
	}
	FRotator DeltaRecoilRotation = FRotator::ZeroRotator;
	FRotator DeltaRecoveryRotation = FRotator::ZeroRotator;
	const FRotator CurrentRotation = GetTargetController()->GetControlRotation();
	
	const FRotator InputLastFrame = CurrentRotation - CachedControllerRotation - RecoilInputGeneratedLastFrame;
	
	CachedControllerRotation = GetTargetController()->GetControlRotation();

	if(!FMath::IsNearlyZero(CurrentRecoilSpeed))
	{
		CurrentRecoilSpeed = FMath::FInterpConstantTo(CurrentRecoilSpeed, 0, DeltaTime, RecoilPattern->RecoilShiftAcceleration);
		DeltaRecoilRotation = FMath::RInterpConstantTo(FRotator::ZeroRotator, RecoilToApply, DeltaTime, CurrentRecoilSpeed);
		if(ProcessDeltaRecoilRotation(DeltaRecoilRotation))
		{
			ApplyInputToController(GetTargetController(), DeltaRecoilRotation);
			RecoilToApply -= DeltaRecoilRotation;
		}
		RecoilToRecover += DeltaRecoilRotation;
	}

	// Unfinished
	//TryApplyRecoilCompensation(InputLastFrame);
	//GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,InputLastFrame.ToString());
		
	if(LastFireTime + DeltaTime + RecoilPattern->RecoveryDelay<GetWorld()->GetTimeSeconds() && !RecoilToRecover.IsNearlyZero())
	{
		CurrentRecoverySpeed = FMath::FInterpConstantTo(CurrentRecoverySpeed, RecoilPattern->MaxRecoverySpeed , DeltaTime, RecoilPattern->RecoveryAcceleration);
		DeltaRecoveryRotation = FMath::RInterpTo( FRotator::ZeroRotator,RecoilToRecover,  DeltaTime, CurrentRecoverySpeed);
		DeltaRecoveryRotation = FRotator(-DeltaRecoveryRotation.Pitch, -DeltaRecoveryRotation.Yaw, 0.f);
		if(ProcessDeltaRecoveryRotation(DeltaRecoveryRotation))
		{
			ApplyInputToController(GetTargetController(), DeltaRecoveryRotation);
			RecoilToRecover += DeltaRecoveryRotation;
		}
	}
	
	RecoilInputGeneratedLastFrame = DeltaRecoilRotation + DeltaRecoveryRotation;
}

void UCRRecoilComponent::SetRecoilPattern(UCRRecoilPattern* InRecoilPattern)
{
	RecoilPattern = InRecoilPattern;
}

void UCRRecoilComponent::StartNewRecoilSequence()
{
	CurrentShotIndex = 0;
}

void UCRRecoilComponent::ApplyShot()
{
	const FVector2f DeltaRecoilLocation = RecoilStrength * RecoilPattern->GetDeltaRecoilLocation(CurrentShotIndex);
	CurrentShotIndex++;
	const float DeltaRecoilLength = DeltaRecoilLocation.Size();
	const float RecoilShiftAcceleration = RecoilPattern->RecoilShiftAcceleration;
	const float RecoilUpliftDuration = RecoilPattern->RecoilUpliftDuration;
	CurrentRecoilSpeed = DeltaRecoilLength / RecoilUpliftDuration + RecoilShiftAcceleration * RecoilUpliftDuration / 2;
	RecoilToApply = VectorToRotator(DeltaRecoilLocation);
	RecoilToRecover = FRotator::ZeroRotator;
	CurrentRecoverySpeed = 0.1f;
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void UCRRecoilComponent::SetRecoilStrength(float InRecoilStrength)
{
	RecoilStrength = FMath::Max(0.05f, InRecoilStrength);
}

float UCRRecoilComponent::GetRecoilStrength(float InRecoilStrength)
{
	return RecoilStrength;
}

FRotator UCRRecoilComponent::VectorToRotator(const FVector2f InputVector)
{
	return FRotator(-InputVector.Y, InputVector.X, 0.f);
}


AController* UCRRecoilComponent::GetTargetController() const
{
	UObject* Owner = GetOwner();
	check(Owner&& Owner->Implements<UCRRecoilInterface>())
	return ICRRecoilInterface::Execute_K2_GetTargetController(Owner);
}

bool UCRRecoilComponent::ProcessDeltaRecoilRotation(FRotator& DeltaRecoilRotation)
{
	// Subclass to do something with Delta Recoil Rotation
	// without to override UpdateAndApplyRecoil
	return true;
}

bool UCRRecoilComponent::ProcessDeltaRecoveryRotation(FRotator& DeltaRecoveryRotation)
{
	// Subclass to do something with Delta Recovery Rotation
	// without to override UpdateAndApplyRecovery
	return true;
}

void UCRRecoilComponent::TryApplyRecoilCompensation(const FRotator& LastFrameInput)
{
	auto CompensateAxis = [](const float RecoveryAxis, const float InputAxis)->float
	{
		const float Sum = RecoveryAxis + InputAxis;
		return FMath::Sign(RecoveryAxis) == FMath::Sign(InputAxis) ? RecoveryAxis :
					FMath::Sign(Sum) == FMath::Sign(RecoveryAxis) ? Sum : 0.f;
	};
	const float CompensatedPitch = CompensateAxis(RecoilToRecover.Pitch, LastFrameInput.Pitch);
	const float CompensatedYaw = CompensateAxis(RecoilToRecover.Yaw, LastFrameInput.Yaw);
	RecoilToRecover.Pitch = CompensatedPitch;
	RecoilToRecover.Yaw = CompensatedYaw;
}

void UCRRecoilComponent::ApplyInputToController(AController* TargetController, const FRotator& Input)
{
	InternalApplyInputToController(TargetController, Input);
}

void UCRRecoilComponent::InternalApplyInputToController(AController* TargetController, const FRotator& Input)
{
	if (TargetController && TargetController->IsLocalPlayerController())
	{
		// AddYawInput is dependent on sensitivity/input scaling
		// We want absolute recoil regardless of settings, so we directly set the control rotation instead of using AddYawInput/AddPitchInput
		FRotator CurrentRotation = TargetController->GetControlRotation();

		// Add the recoil delta
		CurrentRotation.Pitch -= Input.Pitch;
		CurrentRotation.Yaw += Input.Yaw;

		// Normalize to keep values clean (-180 to 180)
		CurrentRotation.Normalize();
		TargetController->SetControlRotation(CurrentRotation);
	}
}
