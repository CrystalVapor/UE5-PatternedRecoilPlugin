// Copyright CrystalVapor 2024, All rights reserved.

#include "CRSpreadRecoilComponent.h"

UCRSpreadRecoilComponent::UCRSpreadRecoilComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCRSpreadRecoilComponent::ApplyShot()
{
	Super::ApplyShot();
	if(ReadyToCalculateRecoil())
	{
		const float ShotHeat = ShotToHeatCurve->Eval(CurrentRecoilHeat);
		AddRecoilHeat(ShotHeat);
	}
}

void UCRSpreadRecoilComponent::AddRecoilHeat(float InHeat)
{
	SetRecoilHeat(GetRecoilHeat()+InHeat);
}

void UCRSpreadRecoilComponent::SetRecoilHeat(float InHeat)
{
	const float CachedCurrentRecoilHeat = CurrentRecoilHeat;
	CurrentRecoilHeat = FMath::Clamp(InHeat, 0.f, MaxRecoilHeat);
	OnHeatChanged.Broadcast(CurrentRecoilHeat, CachedCurrentRecoilHeat);
}

float UCRSpreadRecoilComponent::GetRecoilHeat() const
{
	return CurrentRecoilHeat;
}

void UCRSpreadRecoilComponent::SetMaxRecoilHeat(float InMaxHeat)
{
	MaxRecoilHeat = FMath::Max(0.f, InMaxHeat);
}

void UCRSpreadRecoilComponent::SetRecoilHeatCoolDownDelay(float InDelay)
{
	RecoilHeatCooldownDelay = FMath::Max(0.f, InDelay);
}

void UCRSpreadRecoilComponent::SetShotToHeatCurve(const FRichCurve* InCurve)
{
	ShotToHeatCurve = InCurve;
}

void UCRSpreadRecoilComponent::SetHeatToSpreadAngleCurve(const FRichCurve* InCurve)
{
	HeatToSpreadAngleCurve = InCurve;
}

void UCRSpreadRecoilComponent::SetHeatToCooldownPerSecondCurve(const FRichCurve* InCurve)
{
	HeatToCooldownPerSecondCurve = InCurve;
}

float UCRSpreadRecoilComponent::GetCurrentSpreadAngle() const
{
	check(HeatToSpreadAngleCurve);
	return HeatToSpreadAngleCurve->Eval(CurrentRecoilHeat);
}

void UCRSpreadRecoilComponent::DoHeatCooldown(float DeltaTime)
{
	check(HeatToSpreadAngleCurve)
	const float DeltaCooldown = HeatToCooldownPerSecondCurve->Eval(CurrentRecoilHeat) * DeltaTime;
	SetRecoilHeat(CurrentRecoilHeat - DeltaCooldown);
}

bool UCRSpreadRecoilComponent::ReadyToCalculateRecoil() const
{
	return ShotToHeatCurve && HeatToSpreadAngleCurve && HeatToCooldownPerSecondCurve;
}

void UCRSpreadRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(ReadyToCalculateRecoil()&&LastFireTime + RecoilHeatCooldownDelay < GetWorld()->GetTimeSeconds())
	{
		DoHeatCooldown(DeltaTime);
	}
}

