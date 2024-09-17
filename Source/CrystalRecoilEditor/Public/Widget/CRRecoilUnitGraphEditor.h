// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CRRecoilUnitGraph.h"
#include "CRRecoilUnitGraphWidgetDragOperations.h"
#include "Widgets/SCompoundWidget.h"

#include "CRRecoilUnitGraphEditor.generated.h"

class FCRRecoilUnitSelection;
class UCRRecoilPattern;

namespace CrystalRecoilEditor
{
	enum EEditorLayerOffset
	{
		Base = 10000,
		OriginLayer,
		RecoilUnitsLayer,
		SelectedRecoilUnitsLayer,
		RecoilUnitSelectionBoxLayer,
		RecoilUnitNumbersLayer,
		GridCoordinateLayer,
	};

	const FVector2d UnitDrawSize = FVector2d(30.f, 30.f);
	const FVector2d UnitNumberDrawOffset = FVector2d(12.f, -12.f);
	const FVector2d UnitNumberDrawSize = FVector2d(24.f, 24.f);
	const FVector2d OriginDrawSize = FVector2d(16.f, 16.f);
}

class SCRRecoilUnitGraphBackgroundWidget;
class FCRRecoilPatternEditor;

USTRUCT()
struct FCRRecoilUnitClipboardData
{
	GENERATED_BODY()
	bool ImportFromString(const FString& ImportString)
	{
		FStringOutputDevice errors;
		StaticStruct()->ImportText(*ImportString, this, nullptr, 0, &errors, FCRRecoilUnitClipboardData::StaticStruct()->GetName(), true);
		return errors.IsEmpty();
	}
	UPROPERTY()
	TArray<FVector2D> RecoilUnitLocations;
};

/**
 * 
 */
class CRYSTALRECOILEDITOR_API SCRRecoilUnitGraphWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCRRecoilUnitGraphWidget)
		{
		}
	SLATE_ARGUMENT(FCRRecoilPatternEditor*, RecoilPatternEditor)
	SLATE_END_ARGS()
	enum EAxis
	{
		Axis_X,
		Axis_Y
	};
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetObject(UCRRecoilUnitGraph* InRecoilUnitGraph);
	void RegisterCommands();

	// SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool SupportsKeyboardFocus() const override { return true;}
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	// End of SWidget interface

	void DrawOrigin(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 BaseLayerID) const;
	void DrawRecoilUnits(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 BaseLayerID) const;
	void DrawGridAxisNumbers(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 BaseLayerID) const;
	void DrawSingleGridAxisNumber(int32 LineIndex, float OffsetFromGraphOriginToWidgetCenter, EAxis Axis, const FSlateFontInfo& NumberFontInfo, const TSharedRef<
		                              FSlateFontMeasure>& FontMeasure, int32 GridAxisStep, const FGeometry& AllottedGeometry, FSlateWindowElementList&
	                              OutDrawElements, int32 BaseLayerID) const;
	void DrawSelectionBox(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 BaseLayerID) const;

	void AddUnitUnderCursor();
	void AddUnit(FVector2d RecoilLocation);
	void SetViewPointToGraphOrigin();
	void RearrangeUnits();
	void CopySelectedUnits();
	void PasteUnits();
	void DeleteUnits();
	void StartUnitScaling();
	void StopUnitScaling();

protected:
	UCRRecoilUnitGraph* GetUnitGraph() const;
	UCRRecoilPattern* GetRecoilPattern() const;
	FCRRecoilUnitSelection& GetRecoilUnitSelection() const;

	FVector2D RecoilCoordsToGraphCoords(FVector2D RecoilCoords) const;
	FVector2D GraphCoordsToRecoilCoords(FVector2D GraphCoords) const;
	
	int32 FindUnitByScreenLocation(const FVector2D& ScreenLocation, const FGeometry& MyGeometry) const;

	void SelectUnitsInPanelCoordsRect(FSlateRect& SelectionRect);

	int32 ScaleFromRecoilCoordsToGraphCoords = 16;

	bool bNeedResetViewToOrigin = true;
	
	UCRRecoilUnitGraph* RecoilUnitGraph = nullptr;
	FCRRecoilPatternEditor* RecoilPatternEditor = nullptr;

	TSharedPtr<SCRRecoilUnitGraphBackgroundWidget> BackgroundWidget;

	TOptional<FCRUnitGraphViewDelayedDrag> ViewDrag;
	TOptional<FCRUnitGraphSelectionDelayedDrag> SelectionDrag;
	TOptional<FCRUnitGraphMoveUnitsDelayedDrag> MoveUnitsDrag;
	TOptional<FCRUnitGraphScaleUnitsDelayedDrag> ScaleUnitsDrag;

	int32 LastLeftMouseDownFoundUnitID = INDEX_NONE;
	FVector2d CurrentMousePanelPosition;
};
