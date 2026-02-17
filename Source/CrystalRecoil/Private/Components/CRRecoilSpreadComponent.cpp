// Copyright CrystalVapor 2024, All rights reserved.

#include "Components/CRRecoilSpreadComponent.h"

void UCRRecoilSpreadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (ReadyToCalculateRecoil() && LastFireTime + RecoilHeatCooldownDelay < GetWorld()->GetTimeSeconds())
    {
        DoHeatCooldown(DeltaTime);
    }

    // Re-enable tick if base class disabled it but heat cooldown still needs to run
    if (!FMath::IsNearlyZero(CurrentRecoilHeat) && !IsComponentTickEnabled())
    {
        SetComponentTickEnabled(true);
    }

    // Disable tick only when both heat is cooled and base recoil is complete
    if (FMath::IsNearlyZero(CurrentRecoilHeat) && IsComponentTickEnabled())
    {
        SetComponentTickEnabled(false);
    }
}

void UCRRecoilSpreadComponent::ApplyShot()
{
    Super::ApplyShot(); // Re-enables tick via StartNewRecoilSequence

    if (ReadyToCalculateRecoil())
    {
        AddRecoilHeat(ShotToHeatCurve.GetRichCurveConst()->Eval(CurrentRecoilHeat));
    }
}

void UCRRecoilSpreadComponent::AddRecoilHeat(const float InHeat)
{
    // It's not redundant for external Blueprint calls — if someone calls AddRecoilHeat outside of ApplyShot
    SetComponentTickEnabled(true);
    SetRecoilHeat(GetRecoilHeat() + InHeat);
}

void UCRRecoilSpreadComponent::SetRecoilHeat(const float InHeat)
{
    const float CachedCurrentRecoilHeat = CurrentRecoilHeat;
    CurrentRecoilHeat = FMath::Clamp(InHeat, 0.f, MaxRecoilHeat);
    OnHeatChanged.Broadcast(CurrentRecoilHeat, CachedCurrentRecoilHeat);
}

float UCRRecoilSpreadComponent::GetRecoilHeat() const
{
    return CurrentRecoilHeat;
}

void UCRRecoilSpreadComponent::SetMaxRecoilHeat(const float InMaxHeat)
{
    MaxRecoilHeat = FMath::Max(0.f, InMaxHeat);
}

void UCRRecoilSpreadComponent::SetRecoilHeatCoolDownDelay(const float InDelay)
{
    RecoilHeatCooldownDelay = FMath::Max(0.f, InDelay);
}

float UCRRecoilSpreadComponent::GetCurrentSpreadAngle() const
{
    if (!ensureMsgf(HeatToSpreadAngleCurve.GetRichCurveConst(), TEXT("HeatToSpreadAngleCurve is not set")))
    {
        return 0.f;
    }
    return HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentRecoilHeat);
}

void UCRRecoilSpreadComponent::DoHeatCooldown(const float DeltaTime)
{
    if (!ensureMsgf(HeatToCooldownPerSecondCurve.GetRichCurveConst(), TEXT("HeatToCooldownPerSecondCurve is not set")))
    {
        return;
    }
    const float DeltaCooldown = HeatToCooldownPerSecondCurve.GetRichCurveConst()->Eval(CurrentRecoilHeat) * DeltaTime;
    SetRecoilHeat(CurrentRecoilHeat - DeltaCooldown);
}

bool UCRRecoilSpreadComponent::ReadyToCalculateRecoil() const
{
    return ShotToHeatCurve.GetRichCurveConst() && HeatToSpreadAngleCurve.GetRichCurveConst() && HeatToCooldownPerSecondCurve.GetRichCurveConst();
}
