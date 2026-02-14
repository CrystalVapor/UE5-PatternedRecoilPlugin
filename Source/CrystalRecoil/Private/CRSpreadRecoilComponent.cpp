// Copyright CrystalVapor 2024, All rights reserved.

#include "CRSpreadRecoilComponent.h"

UCRSpreadRecoilComponent::UCRSpreadRecoilComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCRSpreadRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ReadyToCalculateRecoil() && LastFireTime + RecoilHeatCooldownDelay < GetWorld()->GetTimeSeconds())
	{
		DoHeatCooldown(DeltaTime);
	}
}

void UCRSpreadRecoilComponent::ApplyShot()
{
	Super::ApplyShot();

	if (ReadyToCalculateRecoil())
	{
		const float ShotHeat = ShotToHeatCurve->Eval(CurrentRecoilHeat);
		AddRecoilHeat(ShotHeat);
	}
}

void UCRSpreadRecoilComponent::AddRecoilHeat(const float InHeat)
{
	SetRecoilHeat(GetRecoilHeat() + InHeat);
}

void UCRSpreadRecoilComponent::SetRecoilHeat(const float InHeat)
{
	const float CachedCurrentRecoilHeat = CurrentRecoilHeat;
	CurrentRecoilHeat = FMath::Clamp(InHeat, 0.f, MaxRecoilHeat);
	OnHeatChanged.Broadcast(CurrentRecoilHeat, CachedCurrentRecoilHeat);
}

float UCRSpreadRecoilComponent::GetRecoilHeat() const
{
	return CurrentRecoilHeat;
}

void UCRSpreadRecoilComponent::SetMaxRecoilHeat(const float InMaxHeat)
{
	MaxRecoilHeat = FMath::Max(0.f, InMaxHeat);
}

void UCRSpreadRecoilComponent::SetRecoilHeatCoolDownDelay(const float InDelay)
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
	if (!ensureMsgf(HeatToSpreadAngleCurve, TEXT("HeatToSpreadAngleCurve is not set")))
	{
		return 0.f;
	}

	return HeatToSpreadAngleCurve->Eval(CurrentRecoilHeat);
}

void UCRSpreadRecoilComponent::DoHeatCooldown(const float DeltaTime)
{
	check(HeatToCooldownPerSecondCurve)
	const float DeltaCooldown = HeatToCooldownPerSecondCurve->Eval(CurrentRecoilHeat) * DeltaTime;
	SetRecoilHeat(CurrentRecoilHeat - DeltaCooldown);
}

bool UCRSpreadRecoilComponent::ReadyToCalculateRecoil() const
{
	return ShotToHeatCurve && HeatToSpreadAngleCurve && HeatToCooldownPerSecondCurve;
}
