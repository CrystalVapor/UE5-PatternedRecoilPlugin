// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CRRecoilComponent.h"
#include "CRSpreadRecoilComponent.generated.h"

UCLASS()
class CRYSTALRECOIL_API UCRSpreadRecoilComponent : public UCRRecoilComponent
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FCRSpreadRecoilHeatChangedDelegate, float /*NewHeat*/, float /*OldHeat*/);
	
	UCRSpreadRecoilComponent();

	virtual void ApplyShot() override;

	void AddRecoilHeat(float InHeat);

	void SetRecoilHeat(float InHeat);

	UFUNCTION(BlueprintCallable)
	inline float GetRecoilHeat() const;

	void SetMaxRecoilHeat(float InMaxHeat);

	void SetRecoilHeatCoolDownDelay(float InDelay);

	void SetShotToHeatCurve(const FRichCurve* InCurve);

	void SetHeatToSpreadAngleCurve(const FRichCurve* InCurve);

	void SetHeatToCooldownPerSecondCurve(const FRichCurve* InCurve);

	float GetCurrentSpreadAngle() const;

	FCRSpreadRecoilHeatChangedDelegate OnHeatChanged;
	
protected:
	void DoHeatCooldown(float DeltaTime);
	
	bool ReadyToCalculateRecoil() const;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	float MaxRecoilHeat = 100.f;
	float RecoilHeatCooldownDelay = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentRecoilHeat = 0.f;
	
	const FRichCurve* ShotToHeatCurve = nullptr;
	const FRichCurve* HeatToSpreadAngleCurve = nullptr;
	const FRichCurve* HeatToCooldownPerSecondCurve = nullptr;
};
