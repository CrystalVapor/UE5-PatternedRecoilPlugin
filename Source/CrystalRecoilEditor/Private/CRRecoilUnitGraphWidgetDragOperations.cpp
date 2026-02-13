// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilUnitGraphWidgetDragOperations.h"


TArray<FCRRecoilUnit> FCRUnitGraphScaleUnitsDelayedDrag::CacheSelectedUnits(UCRRecoilUnitGraph* UnitGraph,
	FCRRecoilUnitSelection& UnitSelection)
{
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = UnitSelection.GetSelectedRecoilUnits(UnitGraph);
	TArray<FCRRecoilUnit> Ret;
	for(int i = 0; i < SelectedRecoilUnitPtrs.Num(); ++i)
	{
		FCRRecoilUnit* RecoilUnit = SelectedRecoilUnitPtrs[i];
		if(RecoilUnit)
		{
			Ret.Add(*RecoilUnit);
		}
	}
	return Ret;
}

TArray<FCRRecoilUnit> FCRUnitGraphScaleUnitsDelayedDrag::CacheAllUnits(UCRRecoilUnitGraph* UnitGraph)
{
	int32 RecoilUnitCount = UnitGraph->GetUnitCount();
	TArray<FCRRecoilUnit> Ret;
	for(int i = 0;i<RecoilUnitCount;++i)
	{
		Ret.Add(UnitGraph->GetUnitAt(i));
	}
	return Ret;
}

void FCRUnitGraphScaleUnitsDelayedDrag::ApplyCacheToUnitGraph(UCRRecoilUnitGraph* UnitGraph,
	const TArray<FCRRecoilUnit>& InCachedRecoilUnits)
{
	for(int i = 0; i < InCachedRecoilUnits.Num(); ++i)
	{
		FCRRecoilUnit RecoilUnit = InCachedRecoilUnits[i];
		FCRRecoilUnit* UnitPtr = UnitGraph->GetUnitByID(RecoilUnit.ID);
		if(UnitPtr)
		{
			UnitPtr->Location = RecoilUnit.Location;
		}
	}
}

void FCRUnitGraphScaleUnitsDelayedDrag::ApplyScaling(const FCRRecoilUnitSelection& RecoilUnitSelection,
	float NewScale)
{
	NewScale = FMath::Max(0.05f, NewScale);
	float ScaleFactor = NewScale / CurrentScale;
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = RecoilUnitSelection.GetSelectedRecoilUnits(CachedUnitGraph);
	for(int i = 1; i < SelectedRecoilUnitPtrs.Num(); ++i)
	{
		FVector2f VectorFromInitialToUnit = SelectedRecoilUnitPtrs[i]->Location - InitialRecoilLocation;
		VectorFromInitialToUnit *= ScaleFactor;
		SelectedRecoilUnitPtrs[i]->Location = InitialRecoilLocation + VectorFromInitialToUnit;
	}
	CurrentScale = NewScale;
}

void FCRUnitGraphMoveUnitsDelayedDrag::ApplyMovement(FCRRecoilUnitSelection& UnitSelection, FVector2f Movement)
{
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = UnitSelection.GetSelectedRecoilUnits(CachedUnitGraph);
	for(int i = 0; i < SelectedRecoilUnitPtrs.Num(); ++i)
	{
		FCRRecoilUnit* RecoilUnit = SelectedRecoilUnitPtrs[i];
		if(RecoilUnit)
		{
			RecoilUnit->Location += Movement;
		}
	}
}

TArray<FCRRecoilUnit> FCRUnitGraphMoveUnitsDelayedDrag::CacheSelectedUnits(UCRRecoilUnitGraph* UnitGraph, FCRRecoilUnitSelection& UnitSelection)
{
	TArray<FCRRecoilUnit*> SelectedRecoilUnitPtrs = UnitSelection.GetSelectedRecoilUnits(UnitGraph);
	TArray<FCRRecoilUnit> Ret;
	for(int i = 0; i < SelectedRecoilUnitPtrs.Num(); ++i)
	{
		FCRRecoilUnit* RecoilUnit = SelectedRecoilUnitPtrs[i];
		if(RecoilUnit)
		{
			Ret.Add(*RecoilUnit);
		}
	}
	return Ret;
}

TArray<FCRRecoilUnit> FCRUnitGraphMoveUnitsDelayedDrag::CacheAllUnits(UCRRecoilUnitGraph* UnitGraph)
{
	int32 RecoilUnitCount = UnitGraph->GetUnitCount();
	TArray<FCRRecoilUnit> Ret;
	for(int i = 0;i<RecoilUnitCount;++i)
	{
		Ret.Add(UnitGraph->GetUnitAt(i));
	}
	return Ret;
}

void FCRUnitGraphMoveUnitsDelayedDrag::ApplyCacheToUnitGraph(UCRRecoilUnitGraph* UnitGraph,
	const TArray<FCRRecoilUnit>& InCachedRecoilUnits)
{
	for(int i = 0; i < InCachedRecoilUnits.Num(); ++i)
	{
		FCRRecoilUnit RecoilUnit = InCachedRecoilUnits[i];
		FCRRecoilUnit* UnitPtr = UnitGraph->GetUnitByID(RecoilUnit.ID);
		if(UnitPtr)
		{
			UnitPtr->Location = RecoilUnit.Location;
		}
	}
}
