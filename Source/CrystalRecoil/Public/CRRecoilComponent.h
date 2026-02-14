// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRRecoilComponent.generated.h"

class UCRRecoilPattern;
class ICRRecoilInterface;

UCLASS(ClassGroup=(Custom), Meta = (BlueprintSpawnableComponent))
class CRYSTALRECOIL_API UCRRecoilComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCRRecoilComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetRecoilPattern(UCRRecoilPattern* InRecoilPattern);

	// Reset Current shot to zero
	virtual void StartNewRecoilSequence();

	// Apply a new shot to current recoil sequence
	virtual void ApplyShot();

	UFUNCTION(BlueprintCallable, Category = "Recoil Component")
	void SetRecoilStrength(const float InRecoilStrength);

	UFUNCTION(BlueprintCallable, Category = "Recoil Component")
	float GetRecoilStrength() const;

protected:
	static FRotator VectorToRotator(const FVector2f InputVector);

	AController* GetTargetController() const;

	bool ProcessDeltaRecoilRotation(FRotator& DeltaRecoilRotation);

	bool ProcessDeltaRecoveryRotation(FRotator& DeltaRecoveryRotation);

	void TryApplyRecoilCompensation(const FRotator& LastFrameInput);

	virtual void ApplyInputToController(AController* TargetController, const FRotator& Input);

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
	float CurrentUpliftDeceleration = 0.f;
	float LastFireTime = 0.f;
};
