// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CRRecoilPattern.generated.h"

class UCRRecoilUnitGraph;

UENUM()
enum class ERecoilBehaviorOnShotLimitReached
{
	RepeatLast,
	Stop,
	RestartFromCustomIndex,
	Random
};

USTRUCT()
struct FRecoilPatternRandomizedRecoil
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FVector2D RandomXRange = FVector2d(0, 0);
	
	UPROPERTY(EditAnywhere)
	FVector2D RandomYRange = FVector2d(0, 0);
};

/**
 * 
 */
UCLASS()
class CRYSTALRECOIL_API UCRRecoilPattern : public UDataAsset
{
	GENERATED_BODY()
public:
	UCRRecoilPattern();

	FORCEINLINE UCRRecoilUnitGraph* GetUnitGraph(){ return RecoilUnitGraph;}

	// Returns the incremental recoil to be applied compared to the previous shot
	// Clamp the InShotIndex to the valid range
	// e.g. use 1 to get the recoil increment from the shot 0 to shot 1
	// this function auto clamps the shotIndex refer to RecoilBehaviorOnShotLimitReached
	FVector2D GetDeltaRecoilLocation(OUT int& InShotIndex);

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context)const override;
#endif
	
	int32 GetMaxShotIndex() const;

	FVector2D GetDeltaRecoilLocationInternal(int32 InShotIndex);

	UPROPERTY()
	UCRRecoilUnitGraph* RecoilUnitGraph;
	
	UPROPERTY(EditAnywhere)
	ERecoilBehaviorOnShotLimitReached RecoilBehaviorOnShotLimitReached = ERecoilBehaviorOnShotLimitReached::RepeatLast;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "RecoilBehaviorOnShotLimitReached == ERecoilBehaviorOnShotLimitReached::RestartFromCustomIndex", EditConditionHides = true))
	int32 CustomRecoilRestartIndex = 0;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "RecoilBehaviorOnShotLimitReached == ERecoilBehaviorOnShotLimitReached::Random", EditConditionHides = true))
	FRecoilPatternRandomizedRecoil RandomizedRecoil;

	// The time it takes to reach the recoil uplift
	UPROPERTY(EditAnywhere, Category = "Recoil Basics")
	float RecoilUpliftDuration = 0.2f;

	// The Shift Acceleration of the recoil, affects the initial speed of the recoil
	UPROPERTY(EditAnywhere, Category = "Recoil Basics")
	float RecoilShiftAcceleration = 1.0f;
	
	// The Delay before we start the recovery of the recoil since fired
	UPROPERTY(EditAnywhere, Category = "Recoil Basics")
	float RecoveryDelay = 0.5f;
	
	// Max Recovery Speed
	UPROPERTY(EditAnywhere, Category = "Recoil Basics")
	float MaxRecoverySpeed = 1.0f;

	// Acceleration of the recovery
	UPROPERTY(EditAnywhere, Category = "Recoil Basics")
	float RecoveryAcceleration = 1.0f;
};