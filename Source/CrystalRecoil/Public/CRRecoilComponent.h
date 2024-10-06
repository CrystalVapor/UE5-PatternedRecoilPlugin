// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRRecoilComponent.generated.h"

class UCRRecoilPattern;
class ICRRecoilInterface;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSTALRECOIL_API UCRRecoilComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCRRecoilComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetRecoilPattern(UCRRecoilPattern* InRecoilPattern);

	// Reset Current shot to zero
	virtual void StartNewRecoilSequence();
	
	// Apply a new shot to current recoil sequence
	virtual void ApplyShot();

	UFUNCTION(BlueprintCallable)
	void SetRecoilStrength(float InRecoilStrength);
	UFUNCTION(BlueprintCallable)
	float GetRecoilStrength(float InRecoilStrength);

protected:
	static FRotator VectorToRotator(const FVector2D InputVector);

	AController* GetTargetController() const;

	bool ProcessDeltaRecoilRotation(FRotator& DeltaRecoilRotation);
	bool ProcessDeltaRecoveryRotation(FRotator& DeltaRecoveryRotation);

	void TryApplyRecoilCompensation(const FRotator& LastFrameInput);

	virtual void ApplyInputToController(AController* TargetController, const FRotator& Input) const;


	UPROPERTY()
	UCRRecoilPattern* RecoilPattern = nullptr;
	
	float RecoilStrength = 1.f;
	
	FRotator AccumulatedOuterInput = FRotator::ZeroRotator;
	FRotator RecoilInputGeneratedLastFrame = FRotator::ZeroRotator;
	FRotator CachedControllerRotation = FRotator::ZeroRotator;
	
	int32 CurrentShotIndex = 0;
	FRotator RecoilToApply = FRotator::ZeroRotator;
	FRotator RecoilToRecover = FRotator::ZeroRotator;
	float CurrentRecoilSpeed = 0.f;
	float CurrentRecoverySpeed = 0.f;
	
	float LastFireTime = 0.f;
	
private:
	void InternalApplyInputToController(AController* TargetController, const FRotator& Input) const;
};
