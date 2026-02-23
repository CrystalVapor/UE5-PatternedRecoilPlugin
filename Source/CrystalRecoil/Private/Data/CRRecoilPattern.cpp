// Copyright CrystalVapor 2024, All rights reserved.

#include "Data/CRRecoilPattern.h"
#include "Data/CRRecoilUnitGraph.h"

UCRRecoilPattern::UCRRecoilPattern()
{
	RecoilUnitGraph = NewObject<UCRRecoilUnitGraph>(this, "RecoilUnitGraph", RF_Transactional | RF_Public);
}

UCRRecoilUnitGraph* UCRRecoilPattern::GetUnitGraph() const
{
	return RecoilUnitGraph;
}

FVector2f UCRRecoilPattern::ConsumeShot(int32& ShotIndex) const
{
	// Graph units are in "half-degree" space: allows to author values at 2x scale for finer control
	// Multiplying by this value will convert a graph-space delta to actual degrees of camera rotation
	constexpr float GraphToDegrees = 0.5f;

	if (RecoilUnitGraph->GetUnitCount() == 0)
	{
		return FVector2f::ZeroVector;
	}

	if (ShotIndex >= GetMaxShotIndex())
	{
		switch (PatternEndBehavior)
		{
			case ERecoilPatternEndBehavior::Stop:
			{
				return FVector2f::ZeroVector;
			}
			case ERecoilPatternEndBehavior::RepeatLast:
			{
				const int32 MaxIndex = GetMaxShotIndex();
				const FVector2f Current  = RecoilUnitGraph->GetUnitLocationAt(MaxIndex);
				const FVector2f Previous = MaxIndex > 0 ? RecoilUnitGraph->GetUnitLocationAt(MaxIndex - 1) : FVector2f::ZeroVector;
				return (Current - Previous) * GraphToDegrees;
			}
			case ERecoilPatternEndBehavior::RestartFromCustomIndex:
			{
				ShotIndex = FMath::Clamp<int32>(CustomRecoilRestartIndex, 0, GetMaxShotIndex());
				break;
			}
			case ERecoilPatternEndBehavior::Random:
			{
				return FVector2f
				(
					// Non-deterministic: recoil is a local visual-only effect and does not need to match across server/clients
					FMath::RandRange(RandomizedRecoil.RandomXRange.X, RandomizedRecoil.RandomXRange.Y),
					FMath::RandRange(RandomizedRecoil.RandomYRange.X, RandomizedRecoil.RandomYRange.Y)
				) * GraphToDegrees;
			}
		}
	}

	// Normal path (and RestartFromCustomIndex after reset): consume this shot and advance the index
	const FVector2f Current = RecoilUnitGraph->GetUnitLocationAt(ShotIndex);
	const FVector2f Previous = ShotIndex > 0 ? RecoilUnitGraph->GetUnitLocationAt(ShotIndex - 1) : FVector2f::ZeroVector;
	++ShotIndex;
	return (Current - Previous) * GraphToDegrees;
}

int32 UCRRecoilPattern::GetMaxShotIndex() const
{
	return RecoilUnitGraph->GetUnitCount() - 1;
}
