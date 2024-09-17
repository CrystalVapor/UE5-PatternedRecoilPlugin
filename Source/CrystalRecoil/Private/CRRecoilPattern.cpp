// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilPattern.h"

#include "CRRecoilUnitGraph.h"


UCRRecoilPattern::UCRRecoilPattern()
{
	RecoilUnitGraph = NewObject<UCRRecoilUnitGraph>(this, "RecoilUnitGraph", RF_Transactional | RF_Public);
}

FVector2D UCRRecoilPattern::GetDeltaRecoilLocation(int& InShotIndex)
{
	if(RecoilUnitGraph->GetUnitCount() == 0)
	{
		return FVector2d::ZeroVector;
	}
	if(InShotIndex>=GetMaxShotIndex())
	{
		switch (RecoilBehaviorOnShotLimitReached)
		{
		case ERecoilBehaviorOnShotLimitReached::Stop:
			InShotIndex = GetMaxShotIndex();
			return FVector2d::ZeroVector;
		case ERecoilBehaviorOnShotLimitReached::RepeatLast:
			InShotIndex = GetMaxShotIndex();
			break;
		case ERecoilBehaviorOnShotLimitReached::RestartFromCustomIndex:
			InShotIndex = CustomRecoilRestartIndex;
			break;
		case ERecoilBehaviorOnShotLimitReached::Random:
			return 0.5*FVector2d(FMath::RandRange(RandomizedRecoil.RandomXRange.X,
													RandomizedRecoil.RandomXRange.Y),
							FMath::RandRange(RandomizedRecoil.RandomYRange.X,
													RandomizedRecoil.RandomYRange.Y));
		}
	}
	return 0.5 * GetDeltaRecoilLocationInternal(InShotIndex);
}

EDataValidationResult UCRRecoilPattern::IsDataValid(TArray<FText>& ValidationErrors)
{
	return Super::IsDataValid(ValidationErrors);
}

int32 UCRRecoilPattern::GetMaxShotIndex() const
{
	return RecoilUnitGraph->GetUnitCount() - 1;
}

FVector2D UCRRecoilPattern::GetDeltaRecoilLocationInternal(int32 InShotIndex)
{
	const FVector2d RecoilUnit = RecoilUnitGraph->GetUnitLocationAt(InShotIndex);
	const FVector2d RecoilUnitPrevious = InShotIndex > 0 ? RecoilUnitGraph->GetUnitLocationAt(InShotIndex - 1) : FVector2d::ZeroVector;
	const FVector2d RecoilUnitDelta = RecoilUnit - RecoilUnitPrevious;
	return RecoilUnitDelta;
}
