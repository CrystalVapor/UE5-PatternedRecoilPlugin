// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CRRecoilPatternEditor.h"
#include "CRRecoilUnitGraph.h"
#include "Framework/DelayedDrag.h"

class FCRRecoilUnitSelection;
class UCRRecoilUnitGraph;

class FCRUnitGraphViewDelayedDrag: public FDelayedDrag
{
public:
	FCRUnitGraphViewDelayedDrag(const FVector2f InInitialPosition, const FKey& InEffectiveKey)
	 : FDelayedDrag(static_cast<FVector2D>(InInitialPosition), InEffectiveKey)
	{
	}
};

class FCRUnitGraphSelectionDelayedDrag: public FDelayedDrag
{
public:
	FCRUnitGraphSelectionDelayedDrag(const FVector2f InInitialPosition, const FKey& InEffectiveKey)
	 : FDelayedDrag(static_cast<FVector2D>(InInitialPosition), InEffectiveKey)
	{
		TriggerDistance = 0.f;
	}

	FSlateRect GetSelectionRect() const
	{
		const FVector2D LocalInitialPosition = GetInitialPosition();
		const FVector2f LocalEndPosition = EndPosition;
		return FSlateRect(
				FMath::Min(LocalInitialPosition.X, LocalEndPosition.X),
				FMath::Min(LocalInitialPosition.Y, LocalEndPosition.Y),
				FMath::Max(LocalInitialPosition.X, LocalEndPosition.X),
				FMath::Max(LocalInitialPosition.Y, LocalEndPosition.Y)
			);
	}

	FVector2f EndPosition = FVector2f::ZeroVector;
};

class FCRUnitGraphScaleUnitsDelayedDrag
{
public:
	FCRUnitGraphScaleUnitsDelayedDrag(UCRRecoilUnitGraph* UnitGraph, FCRRecoilUnitSelection& UnitSelection, const FVector2f InInitialRecoilLocation ,const FVector2f InInitialPosition, const FKey& InEffectiveKey)
	{
		CachedUnitGraph = UnitGraph;
		CachedRecoilUnits = CacheSelectedUnits(UnitGraph, UnitSelection);
		InitialRecoilLocation = InInitialRecoilLocation;
		InitialPanelLocation = InInitialPosition;
	}
	~FCRUnitGraphScaleUnitsDelayedDrag()
	{
		// first cached moved state and reset to the original state
		TArray<FCRRecoilUnit> MovedRecoilUnits = CacheAllUnits(CachedUnitGraph);
		ApplyCacheToUnitGraph(CachedUnitGraph, CachedRecoilUnits);
		FScopedTransaction Transaction(NSLOCTEXT("CRUnitGraphScaleUnitsDelayedDrag", "DragOperation", "Scale recoil units"));
		// buffer the original state, apply the moved state
		// so that we can undo the move operation
		CachedUnitGraph->Modify();
		ApplyCacheToUnitGraph(CachedUnitGraph, MovedRecoilUnits);
	}
	TArray<FCRRecoilUnit> CacheSelectedUnits(UCRRecoilUnitGraph* UnitGraph, FCRRecoilUnitSelection& UnitSelection);
	TArray<FCRRecoilUnit> CacheAllUnits(UCRRecoilUnitGraph* UnitGraph);
	void ApplyCacheToUnitGraph(UCRRecoilUnitGraph* UnitGraph, const TArray<FCRRecoilUnit>& InCachedRecoilUnits);

	void ApplyScaling(const FCRRecoilUnitSelection& RecoilUnitSelection, float NewScale);

	TArray<FCRRecoilUnit> CachedRecoilUnits;
	UCRRecoilUnitGraph* CachedUnitGraph;
	
	float CurrentScale = 1.f;
	float NormalVectorSizePanel = 1.f;
	FVector2f EndPosition = FVector2f::ZeroVector;
	FVector2f InitialRecoilLocation = FVector2f::ZeroVector;
	FVector2f InitialPanelLocation = FVector2f::ZeroVector;
	int32 BaseUnitID = 0;
};

class FCRUnitGraphMoveUnitsDelayedDrag: public FDelayedDrag
{
public:
	FCRUnitGraphMoveUnitsDelayedDrag(UCRRecoilUnitGraph* UnitGraph, FCRRecoilUnitSelection& UnitSelection, const FVector2f InInitialRecoilLocation, const FVector2f InInitialPosition, const FKey& InEffectiveKey)
	 : FDelayedDrag(static_cast<FVector2D>(InInitialPosition), InEffectiveKey)
	{
		TriggerDistance = 0.f;
		CachedUnitGraph = UnitGraph;
		CachedRecoilUnits = CacheSelectedUnits(UnitGraph, UnitSelection);
		LastRecoilCoordsLocation = InInitialRecoilLocation;
	}
	~FCRUnitGraphMoveUnitsDelayedDrag()
	{
		// first cached moved state and reset to the original state
		TArray<FCRRecoilUnit> MovedRecoilUnits = CacheAllUnits(CachedUnitGraph);
		ApplyCacheToUnitGraph(CachedUnitGraph, CachedRecoilUnits);
		FScopedTransaction Transaction(NSLOCTEXT("CRUnitGraphMoveUnitsDelayedDrag", "DragOperation", "Move recoil units"));
		// buffer the original state, apply the moved state
		// so that we can undo the move operation
		CachedUnitGraph->Modify();
		ApplyCacheToUnitGraph(CachedUnitGraph, MovedRecoilUnits);
	}
	void ApplyMovement(FCRRecoilUnitSelection& UnitSelection, FVector2f Movement);
	
	
	TArray<FCRRecoilUnit> CacheSelectedUnits(UCRRecoilUnitGraph* UnitGraph, FCRRecoilUnitSelection& UnitSelection);
	TArray<FCRRecoilUnit> CacheAllUnits(UCRRecoilUnitGraph* UnitGraph);
	void ApplyCacheToUnitGraph(UCRRecoilUnitGraph* UnitGraph, const TArray<FCRRecoilUnit>& InCachedRecoilUnits);

	TArray<FCRRecoilUnit> CachedRecoilUnits;
	UCRRecoilUnitGraph* CachedUnitGraph;
	
	FVector2f LastRecoilCoordsLocation = FVector2f::ZeroVector;
	FVector2D CachedPannerVector = FVector2d::ZeroVector;
};
