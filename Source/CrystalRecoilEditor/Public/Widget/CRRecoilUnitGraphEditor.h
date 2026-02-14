// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CRRecoilUnitGraph.h"
#include "CRRecoilUnitGraphWidgetDragOperations.h"
#include "Misc/StringOutputDevice.h"
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
		StaticStruct()->ImportText(*ImportString, this, nullptr, 0, &errors, StaticStruct()->GetName(), true);
		return errors.IsEmpty();
	}

	UPROPERTY()
	TArray<FVector2f> RecoilUnitLocations;
};

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
	void RegisterCommands() const;

	// SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	// End of SWidget interface

	void DrawOrigin(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, const int32 BaseLayerID) const;
	void DrawRecoilUnits(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 BaseLayerID) const;
	void DrawGridAxisNumbers(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, const int32 BaseLayerID) const;
	void DrawSingleGridAxisNumber(const int32 LineIndex, const float OffsetFromGraphOriginToWidgetCenter, const EAxis Axis, const FSlateFontInfo& NumberFontInfo, const TSharedRef<FSlateFontMeasure>& FontMeasure, const int32 GridAxisStep, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 BaseLayerID) const;
	void DrawSelectionBox(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, const int32 BaseLayerID) const;

	void AddUnitUnderCursor() const;
	void AddUnit(const FVector2f& RecoilLocation) const;
	void ZoomToFitAllUnits() const;
	void CopySelectedUnits() const;
	void PasteUnits() const;
	void DeleteUnits() const;
	void StartUnitScaling();
	void StopUnitScaling();

protected:
	UCRRecoilUnitGraph* GetUnitGraph() const;
	UCRRecoilPattern* GetRecoilPattern() const;
	FCRRecoilUnitSelection& GetRecoilUnitSelection() const;

	FVector2f RecoilCoordsToGraphCoords(FVector2f RecoilCoords) const;
	FVector2f GraphCoordsToRecoilCoords(FVector2f GraphCoords) const;

	int32 FindUnitByScreenLocation(const FVector2f& ScreenLocation) const;

	void SelectUnitsInPanelCoordsRect(const FSlateRect& SelectionRect) const;

	int32 ScaleFromRecoilCoordsToGraphCoords = 16;

	bool bNeedZoomToFit = false;

	UCRRecoilUnitGraph* RecoilUnitGraph = nullptr;
	FCRRecoilPatternEditor* RecoilPatternEditor = nullptr;

	TSharedPtr<SCRRecoilUnitGraphBackgroundWidget> BackgroundWidget;
	TSharedPtr<SVerticalBox> ShortcutsContainer;

	TOptional<FCRUnitGraphViewDelayedDrag> ViewDrag;
	TOptional<FCRUnitGraphSelectionDelayedDrag> SelectionDrag;
	TOptional<FCRUnitGraphMoveUnitsDelayedDrag> MoveUnitsDrag;
	TOptional<FCRUnitGraphScaleUnitsDelayedDrag> ScaleUnitsDrag;

	int32 LastLeftMouseDownFoundUnitID = INDEX_NONE;
	FVector2f CurrentMousePanelPosition;
};
