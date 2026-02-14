// Copyright CrystalVapor 2024, All rights reserved.

#include "CRRecoilUnitGraphWidgetDragOperations.h"

TArray<FCRRecoilUnit> FCRUnitGraphScaleUnitsDelayedDrag::CacheSelectedUnits(UCRRecoilUnitGraph* UnitGraph, FCRRecoilUnitSelection& UnitSelection)
{
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = UnitSelection.GetSelectedRecoilUnits(UnitGraph);
	TArray<FCRRecoilUnit> Ret;
	for (int32 Index = 0; Index < SelectedRecoilUnitPtrs.Num(); ++Index)
	{
		if (const FCRRecoilUnit* RecoilUnit = SelectedRecoilUnitPtrs[Index])
		{
			Ret.Add(*RecoilUnit);
		}
	}
	return Ret;
}

TArray<FCRRecoilUnit> FCRUnitGraphScaleUnitsDelayedDrag::CacheAllUnits(UCRRecoilUnitGraph* UnitGraph)
{
	const int32 RecoilUnitCount = UnitGraph->GetUnitCount();
	TArray<FCRRecoilUnit> Ret;
	for (int32 Index = 0; Index < RecoilUnitCount; ++Index)
	{
		Ret.Add(UnitGraph->GetUnitAt(Index));
	}
	return Ret;
}

void FCRUnitGraphScaleUnitsDelayedDrag::ApplyCacheToUnitGraph(UCRRecoilUnitGraph* UnitGraph, const TArray<FCRRecoilUnit>& InCachedRecoilUnits)
{
	for (int32 Index = 0; Index < InCachedRecoilUnits.Num(); ++Index)
	{
		const FCRRecoilUnit RecoilUnit = InCachedRecoilUnits[Index];
		if (FCRRecoilUnit* UnitPtr = UnitGraph->GetUnitByID(RecoilUnit.ID))
		{
			UnitPtr->Position = RecoilUnit.Position;
		}
	}
}

void FCRUnitGraphScaleUnitsDelayedDrag::ApplyScaling(const FCRRecoilUnitSelection& RecoilUnitSelection, float NewScale)
{
	NewScale = FMath::Max(0.05f, NewScale);
	const float ScaleFactor = NewScale / CurrentScale;
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = RecoilUnitSelection.GetSelectedRecoilUnits(CachedUnitGraph);
	for (int32 Index = 1; Index < SelectedRecoilUnitPtrs.Num(); ++Index)
	{
		FVector2f VectorFromInitialToUnit = SelectedRecoilUnitPtrs[Index]->Position - InitialRecoilLocation;
		VectorFromInitialToUnit *= ScaleFactor;
		SelectedRecoilUnitPtrs[Index]->Position = InitialRecoilLocation + VectorFromInitialToUnit;
	}
	CurrentScale = NewScale;
}

void FCRUnitGraphMoveUnitsDelayedDrag::ApplyMovement(const FCRRecoilUnitSelection& UnitSelection, const FVector2f& Movement) const
{
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = UnitSelection.GetSelectedRecoilUnits(CachedUnitGraph);
	for (int32 Index = 0; Index < SelectedRecoilUnitPtrs.Num(); ++Index)
	{
		if (FCRRecoilUnit* RecoilUnit = SelectedRecoilUnitPtrs[Index])
		{
			RecoilUnit->Position += Movement;
		}
	}
}

TArray<FCRRecoilUnit> FCRUnitGraphMoveUnitsDelayedDrag::CacheSelectedUnits(UCRRecoilUnitGraph* UnitGraph, const FCRRecoilUnitSelection& UnitSelection)
{
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = UnitSelection.GetSelectedRecoilUnits(UnitGraph);
	TArray<FCRRecoilUnit> Ret;
	for (int32 Index = 0; Index < SelectedRecoilUnitPtrs.Num(); ++Index)
	{
		if (const FCRRecoilUnit* RecoilUnit = SelectedRecoilUnitPtrs[Index])
		{
			Ret.Add(*RecoilUnit);
		}
	}
	return Ret;
}

TArray<FCRRecoilUnit> FCRUnitGraphMoveUnitsDelayedDrag::CacheAllUnits(UCRRecoilUnitGraph* UnitGraph)
{
	const int32 RecoilUnitCount = UnitGraph->GetUnitCount();
	TArray<FCRRecoilUnit> Ret;
	for (int32 Index = 0; Index < RecoilUnitCount; ++Index)
	{
		Ret.Add(UnitGraph->GetUnitAt(Index));
	}
	return Ret;
}

void FCRUnitGraphMoveUnitsDelayedDrag::ApplyCacheToUnitGraph(UCRRecoilUnitGraph* UnitGraph, const TArray<FCRRecoilUnit>& InCachedRecoilUnits)
{
	for (int32 Index = 0; Index < InCachedRecoilUnits.Num(); ++Index)
	{
		const FCRRecoilUnit& RecoilUnit = InCachedRecoilUnits[Index];
		if (FCRRecoilUnit* UnitPtr = UnitGraph->GetUnitByID(RecoilUnit.ID))
		{
			UnitPtr->Position = RecoilUnit.Position;
		}
	}
}
