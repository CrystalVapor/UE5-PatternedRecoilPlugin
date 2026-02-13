// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilPattern.h"

#include "CRRecoilUnitGraph.h"


UCRRecoilPattern::UCRRecoilPattern()
{
	RecoilUnitGraph = NewObject<UCRRecoilUnitGraph>(this, "RecoilUnitGraph", RF_Transactional | RF_Public);
}

FVector2f UCRRecoilPattern::GetDeltaRecoilLocation(int& InShotIndex) const
{
	if(RecoilUnitGraph->GetUnitCount() == 0)
	{
		return FVector2f::ZeroVector;
	}
	if(InShotIndex>=GetMaxShotIndex())
	{
		switch (RecoilBehaviorOnShotLimitReached)
		{
		case ERecoilBehaviorOnShotLimitReached::Stop:
			InShotIndex = GetMaxShotIndex();
			return FVector2f::ZeroVector;
		case ERecoilBehaviorOnShotLimitReached::RepeatLast:
			InShotIndex = GetMaxShotIndex();
			break;
		case ERecoilBehaviorOnShotLimitReached::RestartFromCustomIndex:
			InShotIndex = CustomRecoilRestartIndex;
			break;
		case ERecoilBehaviorOnShotLimitReached::Random:
			return 0.5f * FVector2f(FMath::RandRange(RandomizedRecoil.RandomXRange.X,
													RandomizedRecoil.RandomXRange.Y),
							FMath::RandRange(RandomizedRecoil.RandomYRange.X,
													RandomizedRecoil.RandomYRange.Y));
		}
	}
	return 0.5f * GetDeltaRecoilLocationInternal(InShotIndex);
}

#if WITH_EDITOR
EDataValidationResult UCRRecoilPattern::IsDataValid(FDataValidationContext& Context) const
{
	return Super::IsDataValid(Context);
}
#endif

int32 UCRRecoilPattern::GetMaxShotIndex() const
{
	return RecoilUnitGraph->GetUnitCount() - 1;
}

FVector2f UCRRecoilPattern::GetDeltaRecoilLocationInternal(int32 InShotIndex) const
{
	const FVector2f RecoilUnit = RecoilUnitGraph->GetUnitLocationAt(InShotIndex);
	const FVector2f RecoilUnitPrevious = InShotIndex > 0 ? RecoilUnitGraph->GetUnitLocationAt(InShotIndex - 1) : FVector2f::ZeroVector;
	const FVector2f RecoilUnitDelta = RecoilUnit - RecoilUnitPrevious;
	return RecoilUnitDelta;
}
