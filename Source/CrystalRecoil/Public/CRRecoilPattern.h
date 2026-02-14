// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CRRecoilPattern.generated.h"

class UCRRecoilUnitGraph;

UENUM()
enum class ERecoilPatternEndBehavior : uint8
{
	// Continues applying the last shot's recoil delta (Infinite climb)
	RepeatLast,

	// Recoil stops completely (Gun stabilizes at the last position)
	Stop,

	// Loops the pattern back to a specific shot index
	RestartFromCustomIndex,

	// Switches to procedural random recoil defined in RandomizedRecoil
	Random
};

USTRUCT()
struct FRecoilPatternRandomizedRecoil
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector2D RandomXRange = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector2D RandomYRange = FVector2D::ZeroVector;
};

UCLASS()
class CRYSTALRECOIL_API UCRRecoilPattern : public UDataAsset
{
	GENERATED_BODY()

public:
	UCRRecoilPattern();

	UCRRecoilUnitGraph* GetUnitGraph() const;

	// Returns the incremental recoil to be applied compared to the previous shot
	// Clamp the InShotIndex to the valid range
	// e.g. use 1 to get the recoil increment from the shot 0 to shot 1
	// this function auto clamps the shotIndex refer to PatternEndBehavior
	FVector2f GetDeltaRecoilLocation(int32& InShotIndex) const;

	int32 GetMaxShotIndex() const;

	FVector2f GetDeltaRecoilLocationInternal(const int32 InShotIndex) const;

	UPROPERTY()
	UCRRecoilUnitGraph* RecoilUnitGraph = nullptr;

	/**
	* Controls how fast the recoil kick reaches its peak.
	* 0.0 = Slow, smooth rise (floaty, heavy weapon feel)
	* 1.0 = Instant violent snap (sharp, aggressive kick)
	*/
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 1.f), Category = "Uplift")
	float UpliftSpeed = 0.7f;

	/**
	* Time to wait after the last shot before recovery begins
	*/
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.01f, ForceUnits = "s"), Category = "Recovery")
	float RecoveryDelay = 0.1f;

	/**
	* The starting speed of the return movement
	* High Value: Returns immediately at high speed (Snappy start)
	* Low Value: Eases in slowly (Smooth start)
	*/
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.01f, ForceUnits = "deg/s"), Category = "Recovery")
	float InitialRecoverySpeed = 2.f;

	/**
	* The maximum speed the camera can move while returning to its pre-shot position
	*/
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.01f, ForceUnits = "deg/s"), Category = "Recovery")
	float MaxRecoverySpeed = 10.f;

	/**
	* How fast the recovery speed ramps up (in deg/s²) from Initial to Max
	* High Value: Reaches max speed almost instantly
	* Low Value: Slowly accelerates the return motion (Spongey feel)
	*/
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.01f), Category = "Recovery")
	float RecoveryAcceleration = 40.f;

	/**
	* Defines behavior when the player shoots beyond the defined pattern length
	* RepeatLast: Good for high-recoil weapons (AK-47 style infinite climb)
	* Stop: Good for low recoil weapons that stabilize (Laser beams)
	* Random: Good for heavy weapons (LMGs) with chaotic spray at the end
	*/
	UPROPERTY(EditAnywhere, Category = "Pattern")
	ERecoilPatternEndBehavior PatternEndBehavior = ERecoilPatternEndBehavior::RepeatLast;

	/**
	* The shot index to loop back to when the pattern ends
	* 0: Loops the entire pattern from the start
	* High Value: Loops only the later part of the pattern (The "Sustained Fire" phase)
	*/
	UPROPERTY(EditAnywhere, Meta = (EditCondition = "PatternEndBehavior == ERecoilPatternEndBehavior::RestartFromCustomIndex", EditConditionHides = true), Category = "Pattern")
	int32 CustomRecoilRestartIndex = 0;

	/**
	* Configuration for procedural random recoil
	* Defines the min/max X and Y kick applied after the pattern finishes
	* Used to create unpredictable noise for sustained fire
	*/
	UPROPERTY(EditAnywhere, Meta = (EditCondition = "PatternEndBehavior == ERecoilPatternEndBehavior::Random", EditConditionHides = true), Category = "Pattern")
	FRecoilPatternRandomizedRecoil RandomizedRecoil;
};
