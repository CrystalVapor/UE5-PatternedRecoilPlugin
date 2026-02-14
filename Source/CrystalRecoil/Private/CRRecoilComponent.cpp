// Copyright CrystalVapor 2024, All rights reserved.

#include "CRRecoilComponent.h"
#include "CRRecoilInterface.h"
#include "CRRecoilPattern.h"

UCRRecoilComponent::UCRRecoilComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UCRRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AController* TargetController = GetTargetController();

	if (!RecoilPattern || !TargetController)
	{
		return;
	}

	FRotator DeltaRecoilRotation = FRotator::ZeroRotator;
	FRotator DeltaRecoveryRotation = FRotator::ZeroRotator;
	const FRotator CurrentRotation = TargetController->GetControlRotation();

	const FRotator RotationDelta = (CurrentRotation - CachedControllerRotation).GetNormalized();
	FRotator InputLastFrame = RotationDelta - RecoilInputGeneratedLastFrame;
	InputLastFrame.Normalize();

	CachedControllerRotation = TargetController->GetControlRotation();

	// Apply recoil uplift
	if (!RecoilToApply.IsNearlyZero())
	{
		CurrentRecoilSpeed = FMath::Max(0.f, CurrentRecoilSpeed - CurrentUpliftDeceleration * DeltaTime);

		const float DeltaMove = CurrentRecoilSpeed * DeltaTime;
		const float RemainingMagnitude = FMath::Sqrt(RecoilToApply.Pitch * RecoilToApply.Pitch + RecoilToApply.Yaw * RecoilToApply.Yaw);

		if (DeltaMove >= RemainingMagnitude || FMath::IsNearlyZero(CurrentRecoilSpeed))
		{
			DeltaRecoilRotation = RecoilToApply;
		}
		else
		{
			const float Alpha = DeltaMove / RemainingMagnitude;
			DeltaRecoilRotation = FRotator(RecoilToApply.Pitch * Alpha, RecoilToApply.Yaw * Alpha, 0.f);
		}

		if (ProcessDeltaRecoilRotation(DeltaRecoilRotation))
		{
			ApplyInputToController(TargetController, DeltaRecoilRotation);
			RecoilToApply -= DeltaRecoilRotation;
			RecoilToRecover += DeltaRecoilRotation;
		}
	}

	// Apply recoil recovery - only after uplift is fully complete
	if (RecoilToApply.IsNearlyZero() && LastFireTime + RecoilPattern->RecoveryDelay < GetWorld()->GetTimeSeconds() && !RecoilToRecover.IsNearlyZero(0.001f))
	{
		TryApplyRecoilCompensation(InputLastFrame);

		CurrentRecoverySpeed = FMath::FInterpConstantTo(CurrentRecoverySpeed, RecoilPattern->MaxRecoverySpeed, DeltaTime, RecoilPattern->RecoveryAcceleration);
		DeltaRecoveryRotation = FMath::RInterpTo(FRotator::ZeroRotator, RecoilToRecover, DeltaTime, CurrentRecoverySpeed);
		DeltaRecoveryRotation = FRotator(-DeltaRecoveryRotation.Pitch, -DeltaRecoveryRotation.Yaw, 0.f);

		if (ProcessDeltaRecoveryRotation(DeltaRecoveryRotation))
		{
			ApplyInputToController(TargetController, DeltaRecoveryRotation);
			RecoilToRecover += DeltaRecoveryRotation;
		}

		// Flush tiny remainders to prevent infinite recovery ticks
		if (RecoilToRecover.IsNearlyZero(0.001f))
		{
			RecoilToRecover = FRotator::ZeroRotator;
		}
	}

	// Negate pitch because ApplyInputToController does Pitch -= Input.Pitch (inverted), but Yaw is additive (Yaw += Input.Yaw), so it keeps its sign.
	// Without this, the sign mismatch causes InputLastFrame to see double the recoil as phantom player input, which incorrectly triggers compensation
	RecoilInputGeneratedLastFrame = FRotator(-DeltaRecoilRotation.Pitch - DeltaRecoveryRotation.Pitch, DeltaRecoilRotation.Yaw + DeltaRecoveryRotation.Yaw, 0.f);
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
	if (!RecoilPattern)
	{
		return;
	}

	const FVector2f DeltaRecoilLocation = RecoilStrength * RecoilPattern->GetDeltaRecoilLocation(CurrentShotIndex);
	const float DeltaRecoilLength = DeltaRecoilLocation.Size();

	// Map UpliftSpeed (0-1) to duration: high sharpness = short = snappy
	// Lerp in speed space (1/T) instead of time space so sharpness feels linear
	// At 0.0: 1/2 = 0.5s (floaty)
	// At 0.75: ~20ms (fast)
	// At 1.0: 1/66 = 0.015s (instant)
	constexpr float MinRate = 1.f / 0.5f;
	constexpr float MaxRate = 1.f / 0.025f;
	const float UpliftDuration = 1.f / FMath::Lerp(MinRate, MaxRate, RecoilPattern->UpliftSpeed);

	// Kinematics: v0 = 2d/T, a = 2d/T^2
	// Guarantees camera travels exactly DeltaRecoilLength in exactly UpliftDuration
	CurrentUpliftDeceleration = (2.f * DeltaRecoilLength) / (UpliftDuration * UpliftDuration);
	CurrentRecoilSpeed = 2.f * DeltaRecoilLength / UpliftDuration;

	RecoilToApply = VectorToRotator(DeltaRecoilLocation);
	RecoilToRecover = FRotator::ZeroRotator;
	CurrentRecoverySpeed = RecoilPattern->InitialRecoverySpeed;
	LastFireTime = GetWorld()->GetTimeSeconds();
	++CurrentShotIndex;
}

void UCRRecoilComponent::SetRecoilStrength(const float InRecoilStrength)
{
	RecoilStrength = FMath::Max(0.05f, InRecoilStrength);
}

float UCRRecoilComponent::GetRecoilStrength() const
{
	return RecoilStrength;
}

FRotator UCRRecoilComponent::VectorToRotator(const FVector2f InputVector)
{
	return FRotator(-InputVector.Y, InputVector.X, 0.f);
}

AController* UCRRecoilComponent::GetTargetController() const
{
	const UObject* Owner = GetOwner();
	if (!ensureMsgf(Owner && Owner->Implements<UCRRecoilInterface>(), TEXT("Owner must implement ICRRecoilInterface")))
	{
		return nullptr;
	}
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
	// Only compensate if Player is actively giving input (threshold to avoid noise)
	constexpr float InputThreshold = 0.01f;

	// Compensate Pitch (vertical)
	if (FMath::Abs(LastFrameInput.Pitch) > InputThreshold)
	{
		// Player is pulling down (negative pitch) while we need to recover down (positive RecoilToRecover.Pitch)
		if (FMath::Sign(LastFrameInput.Pitch) != FMath::Sign(RecoilToRecover.Pitch))
		{
			// Reduce recovery by Player input amount, but don't go past zero
			const float NewRecovery = RecoilToRecover.Pitch + LastFrameInput.Pitch;
			if (FMath::Sign(NewRecovery) == FMath::Sign(RecoilToRecover.Pitch))
			{
				RecoilToRecover.Pitch = NewRecovery;
			}
			else
			{
				RecoilToRecover.Pitch = 0.f;
			}
		}
	}

	// Compensate Yaw (horizontal)
	if (FMath::Abs(LastFrameInput.Yaw) > InputThreshold)
	{
		if (FMath::Sign(LastFrameInput.Yaw) != FMath::Sign(RecoilToRecover.Yaw))
		{
			const float NewRecovery = RecoilToRecover.Yaw + LastFrameInput.Yaw;
			if (FMath::Sign(NewRecovery) == FMath::Sign(RecoilToRecover.Yaw))
			{
				RecoilToRecover.Yaw = NewRecovery;
			}
			else
			{
				RecoilToRecover.Yaw = 0.f;
			}
		}
	}
}

void UCRRecoilComponent::ApplyInputToController(AController* TargetController, const FRotator& Input)
{
	if (TargetController && TargetController->IsLocalPlayerController())
	{
		FRotator CurrentRotation = TargetController->GetControlRotation();

		// Apply the recoil delta
		CurrentRotation.Pitch -= Input.Pitch;
		CurrentRotation.Yaw += Input.Yaw;

		// Clamp pitch before normalizing to prevent gimbal lock
		CurrentRotation.Pitch = FMath::ClampAngle(CurrentRotation.Pitch, -89.9f, 89.9f);

		// Normalize yaw to keep it in -180 to 180 range
		CurrentRotation.Normalize();

		TargetController->SetControlRotation(CurrentRotation);
	}
}
