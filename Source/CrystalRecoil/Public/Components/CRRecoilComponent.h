// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRRecoilComponent.generated.h"

class UCRRecoilPattern;
class ICRRecoilInterface;

UCLASS(ClassGroup = (CrystalRecoil), Meta = (BlueprintSpawnableComponent), DisplayName = "Recoil Component")
class CRYSTALRECOIL_API UCRRecoilComponent : public UActorComponent
{
	GENERATED_BODY()

	friend ICRRecoilInterface;

public:
	UCRRecoilComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Assigns the recoil pattern to use for this component
	* Call before StartShooting
	*/
	UFUNCTION(BlueprintCallable, Meta = (AllowAbstract = false), Category = "Recoil Component")
	void SetRecoilPattern(UCRRecoilPattern* InRecoilPattern);

	/**
	* Scales all recoil magnitudes
	* 1.0 = full strength, 0.5 = half, 0.0 = no recoil
	*/
	UFUNCTION(BlueprintCallable, Category = "Recoil Component")
	void SetRecoilStrength(const float InRecoilStrength);

	/** Returns the current recoil strength multiplier */
	UFUNCTION(BlueprintCallable, Category = "Recoil Component")
	float GetRecoilStrength() const;

protected:
	virtual void ApplyInputToController(AController* TargetController, const FRotator& Input);

	virtual void ApplyShot();

	/**
	* Resets recoil state and prepares for a new firing sequence.
	* Called automatically by StartShooting via ICRRecoilInterface.
	* Override in subclasses to reset additional state on fire start.
	*/
	virtual void StartShooting();

	/**
	* Called before each recoil delta is applied to the controller.
	* Override in subclasses to intercept or modify the recoil rotation without overriding the entire tick.
	* Return false to skip applying this delta entirely.
	*
	* Example use cases:
	* - Clamp pitch to prevent recoil pushing past a maximum angle
	* - Scale down recoil while crouching or ADS
	* - Block recoil entirely during cinematics or ability animations
	*/
	virtual bool ProcessDeltaRecoilRotation(FRotator& DeltaRecoilRotation);

	/**
	* Called before each recovery delta is applied to the controller.
	* Override in subclasses to intercept or modify the recovery rotation without overriding the entire tick.
	* Return false to skip applying this delta entirely.
	*
	* Example use cases:
	* - Prevent recovery during specific game states (e.g. cutscenes)
	* - Scale recovery speed based on player state
	* - Log or react to recovery events without touching apply logic
	*/
	virtual bool ProcessDeltaRecoveryRotation(FRotator& DeltaRecoveryRotation);

	/**
	* Reduces the recovery distance if player is already pulling the mouse in the recovery direction
	* Example: Gun kicks up 5°. Player pulls mouse down 2° while recovering.
	* Compensation says "player already moved 2° toward home, so we only need to recover 3° now instead of 5°"
	*/
	void ReduceRecoveryByPlayerInput(const FRotator& LastFrameInput);

	AController* GetTargetController() const;

	static FRotator VectorToRotator(const FVector2f InputVector);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Recoil Component")
	TObjectPtr<UCRRecoilPattern> RecoilPattern;

	// Recoil strength and index parameters
	float RecoilStrength = 1.f;
	int32 CurrentShotIndex = 0;

	// Recoil uplift state
	FRotator RecoilToApply = FRotator::ZeroRotator;
	float CurrentRecoilSpeed = 0.f;
	float CurrentUpliftDeceleration = 0.f;

	// Recovery state
	FRotator RecoilToRecover = FRotator::ZeroRotator;
	float CurrentRecoverySpeed = 0.f;
	float LastFireTime = 0.f;

	// Recovery cancellation tracking
	bool bTrackingInputDuringFire = false;
	FRotator AccumulatedInputDuringFire = FRotator::ZeroRotator;

	// Input tracking for compensation
	FRotator RecoilInputGeneratedLastFrame = FRotator::ZeroRotator;
	FRotator CachedControllerRotation = FRotator::ZeroRotator;

	TWeakObjectPtr<APlayerController> PlayerController;
};
