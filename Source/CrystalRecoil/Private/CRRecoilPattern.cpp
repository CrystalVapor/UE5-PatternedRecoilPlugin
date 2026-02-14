// Copyright CrystalVapor 2024, All rights reserved.

#include "CRRecoilPattern.h"
#include "CRRecoilUnitGraph.h"

UCRRecoilPattern::UCRRecoilPattern()
{
	RecoilUnitGraph = NewObject<UCRRecoilUnitGraph>(this, "RecoilUnitGraph", RF_Transactional | RF_Public);
}

UCRRecoilUnitGraph* UCRRecoilPattern::GetUnitGraph() const
{
	return RecoilUnitGraph;
}

FVector2f UCRRecoilPattern::GetDeltaRecoilLocation(int32 & InShotIndex) const
{
	if (RecoilUnitGraph->GetUnitCount() == 0)
	{
		return FVector2f::ZeroVector;
	}

	if (InShotIndex>=GetMaxShotIndex())
	{
		switch (PatternEndBehavior)
		{
			case ERecoilPatternEndBehavior::Stop:
				InShotIndex = GetMaxShotIndex();
				return FVector2f::ZeroVector;
			case ERecoilPatternEndBehavior::RepeatLast:
				InShotIndex = GetMaxShotIndex();
				break;
			case ERecoilPatternEndBehavior::RestartFromCustomIndex:
				InShotIndex = CustomRecoilRestartIndex;
				break;
			case ERecoilPatternEndBehavior::Random:
				return FVector2f(FMath::RandRange(RandomizedRecoil.RandomXRange.X, RandomizedRecoil.RandomXRange.Y), FMath::RandRange(RandomizedRecoil.RandomYRange.X, RandomizedRecoil.RandomYRange.Y)) * 0.5f;
		}
	}

	return GetDeltaRecoilLocationInternal(InShotIndex) * 0.5f;
}

int32 UCRRecoilPattern::GetMaxShotIndex() const
{
	return RecoilUnitGraph->GetUnitCount() - 1;
}

FVector2f UCRRecoilPattern::GetDeltaRecoilLocationInternal(const int32 InShotIndex) const
{
	const FVector2f RecoilUnit = RecoilUnitGraph->GetUnitLocationAt(InShotIndex);
	const FVector2f RecoilUnitPrevious = InShotIndex > 0 ? RecoilUnitGraph->GetUnitLocationAt(InShotIndex - 1) : FVector2f::ZeroVector;
	const FVector2f RecoilUnitDelta = RecoilUnit - RecoilUnitPrevious;
	return RecoilUnitDelta;
}
