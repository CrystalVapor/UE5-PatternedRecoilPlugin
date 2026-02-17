// Copyright CrystalVapor 2024, All rights reserved.

#include "Editor/CRRecoilUnitGraphWidgetDragOperations.h"

namespace
{
	TArray<FCRRecoilUnit> CacheSelectedUnits(UCRRecoilUnitGraph* UnitGraph, const FCRRecoilUnitSelection& UnitSelection)
	{
		TArray<FCRRecoilUnit> Result;
		for (const FCRRecoilUnit* RecoilUnit : UnitSelection.GetSelectedRecoilUnits(UnitGraph))
		{
			if (RecoilUnit)
			{
				Result.Add(*RecoilUnit);
			}
		}
		return Result;
	}

	TArray<FCRRecoilUnit> CacheAllUnits(UCRRecoilUnitGraph* UnitGraph)
	{
		TArray<FCRRecoilUnit> Result;
		for (int32 Index = 0; Index < UnitGraph->GetUnitCount(); ++Index)
		{
			Result.Add(UnitGraph->GetUnitAt(Index));
		}
		return Result;
	}

	void ApplyCacheToUnitGraph(UCRRecoilUnitGraph* UnitGraph, const TArray<FCRRecoilUnit>& CachedUnits)
	{
		for (const FCRRecoilUnit& RecoilUnit : CachedUnits)
		{
			if (FCRRecoilUnit* UnitPtr = UnitGraph->GetUnitByID(RecoilUnit.ID))
			{
				UnitPtr->Position = RecoilUnit.Position;
			}
		}
	}
}

FCRUnitGraphScaleUnitsDelayedDrag::FCRUnitGraphScaleUnitsDelayedDrag(UCRRecoilUnitGraph* UnitGraph, const FCRRecoilUnitSelection& UnitSelection, const FVector2f InInitialRecoilLocation, const FVector2f InInitialPosition, const FKey& InEffectiveKey)
{
	CachedUnitGraph = UnitGraph;
	CachedRecoilUnits = CacheSelectedUnits(UnitGraph, UnitSelection);
	InitialRecoilLocation = InInitialRecoilLocation;
	InitialPanelLocation = InInitialPosition;
}

FCRUnitGraphScaleUnitsDelayedDrag::~FCRUnitGraphScaleUnitsDelayedDrag()
{
	const TArray<FCRRecoilUnit> MovedRecoilUnits = CacheAllUnits(CachedUnitGraph);
	ApplyCacheToUnitGraph(CachedUnitGraph, CachedRecoilUnits);
	FScopedTransaction Transaction(NSLOCTEXT("CRUnitGraphScaleUnitsDelayedDrag", "DragOperation", "Scale recoil units"));
	CachedUnitGraph->Modify();
	ApplyCacheToUnitGraph(CachedUnitGraph, MovedRecoilUnits);
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

FCRUnitGraphMoveUnitsDelayedDrag::FCRUnitGraphMoveUnitsDelayedDrag(UCRRecoilUnitGraph* UnitGraph, const FCRRecoilUnitSelection& UnitSelection, const FVector2f InInitialRecoilLocation, const FVector2f InInitialPosition, const FKey& InEffectiveKey)
	: FDelayedDrag(static_cast<FVector2D>(InInitialPosition), InEffectiveKey)
{
	TriggerDistance = 0.f;
	CachedUnitGraph = UnitGraph;
	CachedRecoilUnits = CacheSelectedUnits(UnitGraph, UnitSelection);
	LastRecoilCoordsLocation = InInitialRecoilLocation;
}

FCRUnitGraphMoveUnitsDelayedDrag::~FCRUnitGraphMoveUnitsDelayedDrag()
{
	const TArray<FCRRecoilUnit> MovedRecoilUnits = CacheAllUnits(CachedUnitGraph);
	ApplyCacheToUnitGraph(CachedUnitGraph, CachedRecoilUnits);
	FScopedTransaction Transaction(NSLOCTEXT("CRUnitGraphMoveUnitsDelayedDrag", "DragOperation", "Move recoil units"));
	CachedUnitGraph->Modify();
	ApplyCacheToUnitGraph(CachedUnitGraph, MovedRecoilUnits);
}

void FCRUnitGraphMoveUnitsDelayedDrag::ApplyMovement(const FCRRecoilUnitSelection& UnitSelection, const FVector2f& Movement) const
{
	for (FCRRecoilUnit* RecoilUnit : UnitSelection.GetSelectedRecoilUnits(CachedUnitGraph))
	{
		if (RecoilUnit)
		{
			RecoilUnit->Position += Movement;
		}
	}
}
