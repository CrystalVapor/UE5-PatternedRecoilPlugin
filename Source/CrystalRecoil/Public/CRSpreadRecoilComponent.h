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

	void AddRecoilHeat(const float InHeat);

	void SetRecoilHeat(const float InHeat);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRecoilHeat() const;

	void SetMaxRecoilHeat(const float InMaxHeat);

	void SetRecoilHeatCoolDownDelay(const float InDelay);

	void SetShotToHeatCurve(const FRichCurve* InCurve);

	void SetHeatToSpreadAngleCurve(const FRichCurve* InCurve);

	void SetHeatToCooldownPerSecondCurve(const FRichCurve* InCurve);

	float GetCurrentSpreadAngle() const;

	FCRSpreadRecoilHeatChangedDelegate OnHeatChanged;
	
protected:
	void DoHeatCooldown(const float DeltaTime);
	
	bool ReadyToCalculateRecoil() const;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentRecoilHeat = 0.f;

	float MaxRecoilHeat = 100.f;

	float RecoilHeatCooldownDelay = 0.5f;

	const FRichCurve* ShotToHeatCurve = nullptr;

	const FRichCurve* HeatToSpreadAngleCurve = nullptr;

	const FRichCurve* HeatToCooldownPerSecondCurve = nullptr;
};
