// Copyright CrystalVapor 2024, All rights reserved.


#include "Widget/CRRecoilUnitGraphEditor.h"

#include "CRRecoilPattern.h"
#include "CRRecoilPatternEditor.h"
#include "CRRecoilPatternEditorCommands.h"
#include "Fonts/FontMeasure.h"
#include "Widget/CRRecoilUnitGraphBackgroundWidget.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

void SCRRecoilUnitGraphWidget::Construct(const FArguments& InArgs)
{
	check(InArgs._RecoilPatternEditor)
	RecoilPatternEditor = InArgs._RecoilPatternEditor;

	RegisterCommands();

	const FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 11);

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(BackgroundWidget, SCRRecoilUnitGraphBackgroundWidget)
			.Visibility(EVisibility::HitTestInvisible)
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		[
			SNew(SOverlay)
			+SOverlay::Slot() // Editor Status Bar Background
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush(TEXT("Graph.TitleBackground")))
			]
			+SOverlay::Slot() // 
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(8.f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("This is a Status bar!")))
				]
			]
		]
	];
}

void SCRRecoilUnitGraphWidget::SetObject(UCRRecoilUnitGraph* InRecoilUnitGraph)
{
	RecoilUnitGraph = InRecoilUnitGraph;
}

void SCRRecoilUnitGraphWidget::SetViewPointToGraphOrigin()
{
	BackgroundWidget->SetViewOffset(BackgroundWidget->GetTickSpaceGeometry().GetLocalSize()*-0.5f/BackgroundWidget->GetZoomAmount());
}

void SCRRecoilUnitGraphWidget::RegisterCommands()
{
	const FCRRecoilPatternEditorCommands& Commands = FCRRecoilPatternEditorCommands::Get();
	const TSharedPtr<FUICommandList> CommandList = RecoilPatternEditor->GetToolkitCommands();
	
}

int32 SCRRecoilUnitGraphWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                        const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{

	
	if(RecoilPatternEditor&&RecoilPatternEditor->bEnableAutoRearrangeUnits)
	{
		if(RecoilUnitGraph)
		{
			RecoilUnitGraph->RearrangeUnits();
		}
	}

	if(bNeedResetViewToOrigin)
	{
		const_cast<SCRRecoilUnitGraphWidget&>(*this).SetViewPointToGraphOrigin();
		const_cast<SCRRecoilUnitGraphWidget&>(*this).bNeedResetViewToOrigin = false;
	}
	
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
	DrawOrigin(OutDrawElements, AllottedGeometry, LayerId);
	DrawRecoilUnits(OutDrawElements, AllottedGeometry, LayerId);
	DrawGridAxisNumbers(OutDrawElements, AllottedGeometry, LayerId);
	DrawSelectionBox(OutDrawElements, AllottedGeometry, LayerId);
	return LayerId;
}

FReply SCRRecoilUnitGraphWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SelectionDrag.Reset();

	FVector2D MousePanelLocation = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	if(MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		ViewDrag = FCRUnitGraphViewDelayedDrag(
			MousePanelLocation,
			EKeys::RightMouseButton);
	}
	if(MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		LastLeftMouseDownFoundUnitID = FindUnitByScreenLocation(MousePanelLocation, MyGeometry);
		FCRRecoilUnitSelection& CurrentRecoilUnitSelection = GetRecoilUnitSelection();
		if(LastLeftMouseDownFoundUnitID==INDEX_NONE)
		{
			CurrentRecoilUnitSelection.ClearSelection();
			SelectionDrag = FCRUnitGraphSelectionDelayedDrag(
			   MousePanelLocation,
			   EKeys::LeftMouseButton);
			MoveUnitsDrag.Reset();
		}else
		{
			if(!MouseEvent.IsControlDown()&&!CurrentRecoilUnitSelection.IsUnitSelected(LastLeftMouseDownFoundUnitID))
			{
				CurrentRecoilUnitSelection.ClearSelection();
			}
			CurrentRecoilUnitSelection.AddSelection(LastLeftMouseDownFoundUnitID);
			MoveUnitsDrag = FCRUnitGraphMoveUnitsDelayedDrag(
				GetUnitGraph(),
				CurrentRecoilUnitSelection,
				GraphCoordsToRecoilCoords(BackgroundWidget->PanelCoordToGraphCoord(MousePanelLocation)),
				MousePanelLocation,
				EKeys::LeftMouseButton);
		}
	}
	return FReply::Handled();
}

FReply SCRRecoilUnitGraphWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if(ViewDrag.IsSet()&&ViewDrag->IsDragging())
		{
			ViewDrag.Reset();
		}
	}
	else if(MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bool bLeftMouseUpWithNoDrag = true;
		if(SelectionDrag.IsSet()&&SelectionDrag->IsDragging())
		{
			SelectionDrag.Reset();
			bLeftMouseUpWithNoDrag = false;
		}
		if(MoveUnitsDrag.IsSet()&&MoveUnitsDrag->IsDragging())
        {
         	MoveUnitsDrag.Reset();
			bLeftMouseUpWithNoDrag = false;
        }
		if(bLeftMouseUpWithNoDrag&&LastLeftMouseDownFoundUnitID==INDEX_NONE)
        {
        	if(!MouseEvent.IsControlDown())
        	{
        		FCRRecoilUnitSelection& CurrentRecoilUnitSelection = GetRecoilUnitSelection();
        		CurrentRecoilUnitSelection.ClearSelection();
        	}
        }
	}
	return FReply::Handled();
}

FReply SCRRecoilUnitGraphWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	CurrentMousePanelPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	if(ViewDrag.IsSet()&&(ViewDrag->IsDragging()||ViewDrag->AttemptDragStart(MouseEvent)))
	{
		BackgroundWidget->SetViewOffset(BackgroundWidget->GetViewOffset() - MouseEvent.GetCursorDelta() / BackgroundWidget->GetZoomAmount());
	}

	if(SelectionDrag.IsSet() && (SelectionDrag->IsDragging() || SelectionDrag->AttemptDragStart(MouseEvent)))
	{
		SelectionDrag->EndPosition = CurrentMousePanelPosition;
		FSlateRect SelectionRect = SelectionDrag->GetSelectionRect();
		SelectUnitsInPanelCoordsRect(SelectionRect);
	}

	if(MoveUnitsDrag.IsSet() && (MoveUnitsDrag->IsDragging() || MoveUnitsDrag->AttemptDragStart(MouseEvent)))
	{
		const FVector2d NewPanelLocation = CurrentMousePanelPosition;
		const FVector2d NewGraphLocation = BackgroundWidget->PanelCoordToGraphCoord(NewPanelLocation);
		const FVector2d NewRecoilLocation = GraphCoordsToRecoilCoords(NewGraphLocation);
		const FVector2d LastRecoilLocation = MoveUnitsDrag->LastRecoilCoordsLocation;
		
		const bool bEnableGridSnapping = RecoilPatternEditor->bEnableGridSnapping;
	    const float GridSnapSize = RecoilPatternEditor->GridSnappingValue;
		const FCRRecoilUnitSelection& RecoilUnitSelection = GetRecoilUnitSelection();
		if(!bEnableGridSnapping)
		{
			const FVector2d DeltaRecoilLocation = NewRecoilLocation - LastRecoilLocation;
			MoveUnitsDrag->ApplyMovement(const_cast<FCRRecoilUnitSelection&>(RecoilUnitSelection), DeltaRecoilLocation);
			MoveUnitsDrag->LastRecoilCoordsLocation = NewRecoilLocation;
		}
		else
		{
			UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();
			const FCRRecoilUnit& LastRecoilUnit = *(CurrentRecoilUnitGraph->GetUnitByID(LastLeftMouseDownFoundUnitID));
			const FVector2d SnappedNewRecoilLocation = FVector2d(
				FMath::RoundToInt(NewRecoilLocation.X / GridSnapSize) * GridSnapSize,
				FMath::RoundToInt(NewRecoilLocation.Y / GridSnapSize) * GridSnapSize);
			const FVector2d DeltaRecoilLocation = SnappedNewRecoilLocation - LastRecoilUnit.Location;
			MoveUnitsDrag->ApplyMovement(const_cast<FCRRecoilUnitSelection&>(RecoilUnitSelection), DeltaRecoilLocation);
			MoveUnitsDrag->LastRecoilCoordsLocation = SnappedNewRecoilLocation;
		}
		
	}

	if(ScaleUnitsDrag.IsSet())
	{
		const FVector2d NewPanelLocation = CurrentMousePanelPosition;
		const FVector2d InitialPanelLocation = ScaleUnitsDrag->InitialPanelLocation;
		const FVector2d ScaleVector = NewPanelLocation - InitialPanelLocation;
		const float NewScale = ScaleVector.Size() / ScaleUnitsDrag->NormalVectorSizePanel;
		const FCRRecoilUnitSelection& RecoilUnitSelection = GetRecoilUnitSelection();
		ScaleUnitsDrag->ApplyScaling(RecoilUnitSelection, NewScale);
	}
	
	return SCompoundWidget::OnMouseMove(MyGeometry, MouseEvent);
}

FReply SCRRecoilUnitGraphWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return BackgroundWidget->OnMouseWheel(MyGeometry, MouseEvent);
}

void SCRRecoilUnitGraphWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	if(ViewDrag.IsSet()&&ViewDrag->IsDragging())
	{
		ViewDrag.Reset();
	}
	if(SelectionDrag.IsSet()&&SelectionDrag->IsDragging())
	{
		SelectionDrag.Reset();
	}
	SCompoundWidget::OnMouseLeave(MouseEvent);
}

FReply SCRRecoilUnitGraphWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return SCompoundWidget::OnKeyUp(MyGeometry, InKeyEvent);
}

void SCRRecoilUnitGraphWidget::DrawOrigin(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
	int32 BaseLayerID) const
{
	FVector2d OriginGraphCoords = FVector2d::ZeroVector;
	FVector2d OriginPanelCoords = BackgroundWidget->GraphCoordToPanelCoord(OriginGraphCoords);
	const FVector2d OriginDrawSize = CrystalRecoilEditor::OriginDrawSize;
	const FVector2d OriginDrawOffset = OriginDrawSize/2;
	const FSlateRect OriginDrawRect = FSlateRect(OriginPanelCoords - OriginDrawOffset, OriginPanelCoords + OriginDrawOffset);
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		BaseLayerID + CrystalRecoilEditor::EEditorLayerOffset::OriginLayer,
		AllottedGeometry.ToPaintGeometry(OriginDrawSize, FSlateLayoutTransform(OriginPanelCoords - OriginDrawOffset)),
		FAppStyle::GetBrush(TEXT("Graph.VarNode.ShadowSelected")),
		ESlateDrawEffect::None,
		FLinearColor::White.CopyWithNewOpacity(0.25));
}

void SCRRecoilUnitGraphWidget::DrawRecoilUnits(FSlateWindowElementList& OutDrawElements,
                                               const FGeometry& AllottedGeometry, int32 BaseLayerID) const
{
	const UCRRecoilPattern*			CurrentRecoilPattern = GetRecoilPattern();
	const UCRRecoilUnitGraph*		CurrentRecoilUnitGraph = GetUnitGraph();
	
	const int32						UnitCount = CurrentRecoilUnitGraph->GetUnitCount();
	const FVector2D					UnitDrawSize = CrystalRecoilEditor::UnitDrawSize;
	const FCRRecoilUnitSelection&	UnitSelection = GetRecoilUnitSelection();
	const int32						UnitLayerID = BaseLayerID + CrystalRecoilEditor::EEditorLayerOffset::RecoilUnitsLayer;
	const int32						UnitNumberLayerID = BaseLayerID + CrystalRecoilEditor::EEditorLayerOffset::RecoilUnitNumbersLayer;
	const SCRRecoilUnitGraphBackgroundWidget* CurrentBackgroundWidget = this->BackgroundWidget.Get();
	
	const FSlateFontInfo NumberFontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 10);
	
	const bool bDrawUnitLines = CurrentRecoilUnitGraph->bDrawUnitLines;
	const bool bDrawUnitNumbers = CurrentRecoilUnitGraph->bDrawUnitNumbers
		&& (CurrentBackgroundWidget->GetCurrentLOD() >= EGraphRenderingLOD::MediumDetail);
	
	for(int32 i = 0; i< UnitCount; i++)
	{
		const FCRRecoilUnit& RecoilUnit = CurrentRecoilUnitGraph->GetUnitAt(i);
		const FVector2D RecoilUnitCenterGraphLocation = RecoilCoordsToGraphCoords(RecoilUnit.Location);
		const FVector2D RecoilUnitDrawGraphLocation = RecoilUnitCenterGraphLocation;
		const FVector2D RecoilUnitDrawPanelLocation = CurrentBackgroundWidget->GraphCoordToPanelCoord(RecoilUnitDrawGraphLocation) - UnitDrawSize / 2.f;
		const bool bIsSelected = UnitSelection.IsUnitSelected(RecoilUnit.ID);
		const FLinearColor UnitColor = bIsSelected ? FLinearColor::Green : FLinearColor::Yellow;

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			UnitLayerID,
			AllottedGeometry.ToPaintGeometry(UnitDrawSize, FSlateLayoutTransform(RecoilUnitDrawPanelLocation)),
			FAppStyle::GetBrush("Graph.VarNode.ShadowSelected"),
			ESlateDrawEffect::None,
			UnitColor);

		if(bDrawUnitNumbers)
		{
			const FVector2d& UnitNumberDrawOffset = CrystalRecoilEditor::UnitNumberDrawOffset;
			const FVector2d& UnitNumberDrawSize = CrystalRecoilEditor::UnitNumberDrawSize;
			FSlateDrawElement::MakeText(
				OutDrawElements,
				UnitNumberLayerID,
				AllottedGeometry.ToPaintGeometry(UnitNumberDrawSize, FSlateLayoutTransform(RecoilUnitDrawPanelLocation + UnitNumberDrawOffset + UnitNumberDrawSize / 2.f)),
				FString::FromInt(i),
				NumberFontInfo,
				ESlateDrawEffect::None,
				FLinearColor::Red);
		}

		if(bDrawUnitLines && i != 0)
		{
			const FCRRecoilUnit& PreviousRecoilUnit = CurrentRecoilUnitGraph->GetUnitAt(i-1);
			const FVector2D PreviousRecoilUnitCenterGraphLocation = RecoilCoordsToGraphCoords(PreviousRecoilUnit.Location);
			const FVector2D PreviousRecoilUnitCenterPanelLocation = CurrentBackgroundWidget->GraphCoordToPanelCoord(PreviousRecoilUnitCenterGraphLocation);
			const FVector2D RecoilUnitCenterPanelLocation = CurrentBackgroundWidget->GraphCoordToPanelCoord(RecoilUnitCenterGraphLocation);
			FSlateDrawElement::MakeLines(
				OutDrawElements,
				UnitLayerID,
				AllottedGeometry.ToPaintGeometry(),
				{PreviousRecoilUnitCenterPanelLocation, RecoilUnitCenterPanelLocation},
				ESlateDrawEffect::None,
				FLinearColor::White.CopyWithNewOpacity(0.7f),
				true,
				2.f);
		}
	}
}

void SCRRecoilUnitGraphWidget::DrawGridAxisNumbers(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 BaseLayerID) const
{
	const FVector2d GraphOriginViewOffset = BackgroundWidget->GetZoomedAndCenterBasedViewOffset();
	const FVector2D LocalSize = GetTickSpaceGeometry().GetLocalSize();
	const int32 GridAxisStep = BackgroundWidget->GetGridAxisStep();
	const float MaxWindowLength = FMath::Max(LocalSize.X, LocalSize.Y);
	const int32 TotalDrawnGridLines = FMath::CeilToInt(MaxWindowLength / ScaleFromRecoilCoordsToGraphCoords);
	const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	const FSlateFontInfo NumberFontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 10);

	// Draw Center
	DrawSingleGridAxisNumber(0, GraphOriginViewOffset.X, Axis_X,
			NumberFontInfo, FontMeasure, GridAxisStep,
			AllottedGeometry, OutDrawElements, BaseLayerID);
	DrawSingleGridAxisNumber(0, GraphOriginViewOffset.Y, Axis_Y,
			NumberFontInfo, FontMeasure, GridAxisStep,
			AllottedGeometry, OutDrawElements, BaseLayerID);
	
	// Draw Others
	for(int32 i = 1; i<=TotalDrawnGridLines; i++)
	{
		DrawSingleGridAxisNumber(i, GraphOriginViewOffset.X, Axis_X,
			NumberFontInfo, FontMeasure, GridAxisStep,
			AllottedGeometry, OutDrawElements, BaseLayerID);
		DrawSingleGridAxisNumber(-i, GraphOriginViewOffset.X, Axis_X,
			NumberFontInfo, FontMeasure, GridAxisStep,
			AllottedGeometry, OutDrawElements, BaseLayerID);
		DrawSingleGridAxisNumber(i, GraphOriginViewOffset.Y, Axis_Y,
			NumberFontInfo, FontMeasure, GridAxisStep,
			AllottedGeometry, OutDrawElements, BaseLayerID);
		DrawSingleGridAxisNumber(-i, GraphOriginViewOffset.Y, Axis_Y,
			NumberFontInfo, FontMeasure, GridAxisStep,
			AllottedGeometry, OutDrawElements, BaseLayerID);
	}
}

void SCRRecoilUnitGraphWidget::DrawSingleGridAxisNumber(int32 LineIndex, float OffsetFromGraphOriginToWidgetCenter, EAxis Axis, const FSlateFontInfo& NumberFontInfo, const TSharedRef<
	                                                        FSlateFontMeasure>& FontMeasure, int32 GridAxisStep, const FGeometry& AllottedGeometry, FSlateWindowElementList&
                                                        OutDrawElements, int32 BaseLayerID) const
{
	// GridLineNumberBiasFromCenter is static in one paint call
	const int32 GridLineNumberOffsetFromGraphCenter = FMath::FloorToInt(OffsetFromGraphOriginToWidgetCenter / ScaleFromRecoilCoordsToGraphCoords / GridAxisStep) * GridAxisStep;
	const int32 GridLineNumber = LineIndex * GridAxisStep + GridLineNumberOffsetFromGraphCenter;

	const FVector2d GridLineNumberGraphCoords = Axis==Axis_X ?
					FVector2d(GridLineNumber * ScaleFromRecoilCoordsToGraphCoords, 0.f) :
					FVector2d(0.f, GridLineNumber * ScaleFromRecoilCoordsToGraphCoords);

	FVector2d GridLineNumberPanelCoords = BackgroundWidget->GraphCoordToPanelCoord(GridLineNumberGraphCoords);
	if(Axis == Axis_X)
	{
		GridLineNumberPanelCoords.Y = 0.f;
	}
	else
	{
		GridLineNumberPanelCoords.X = 0.f;
	}

	FSlateRect SafeZoneRect(-50.f, -50.f, 10.f, 10.f);
	// the tick may be overlapped, skip them.
	if(SafeZoneRect.ContainsPoint(GridLineNumberPanelCoords))
	{
		return;
	}
	
	// we want Y axis to be drawn from bottom to top
	// more familiar to the user, not the screen coordinates
	const FString GridTickText = Axis == Axis_Y ?
		FString::FromInt(-GridLineNumber) :
		FString::FromInt(GridLineNumber);
	const FLinearColor GridTickTextColor = GridLineNumber == 0 ?
		FLinearColor::White :
		FLinearColor::White.CopyWithNewOpacity(0.65f);
	const FVector2d GridTickTextSize = FontMeasure->Measure(GridTickText, NumberFontInfo);
	FVector2d GridTickDrawnPosition = GridLineNumberPanelCoords;
	if(Axis == Axis_X)
	{
		GridTickDrawnPosition.X -= GridTickTextSize.X / 2.f;
	}
	else
	{
		GridTickDrawnPosition.Y -= GridTickTextSize.Y / 2.f;
	}

	FSlateLayoutTransform TextTransform = FSlateLayoutTransform(GridTickDrawnPosition);
	const FPaintGeometry TextPaintGeometry = AllottedGeometry.ToPaintGeometry(GridTickTextSize, TextTransform);
	FSlateDrawElement::MakeText(
		OutDrawElements,
		BaseLayerID + CrystalRecoilEditor::EEditorLayerOffset::GridCoordinateLayer,
		TextPaintGeometry,
		GridTickText,
		NumberFontInfo,
		ESlateDrawEffect::None,
		GridTickTextColor);
}

void SCRRecoilUnitGraphWidget::DrawSelectionBox(FSlateWindowElementList& OutDrawElements,
	const FGeometry& AllottedGeometry, int32 BaseLayerID) const
{
	if(!SelectionDrag.IsSet() || !SelectionDrag->IsDragging())
	{
		return;
	}
	const FSlateRect SelectionBoxRect = SelectionDrag->GetSelectionRect();
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		BaseLayerID + CrystalRecoilEditor::EEditorLayerOffset::RecoilUnitSelectionBoxLayer,
		AllottedGeometry.ToPaintGeometry(SelectionBoxRect.GetSize(), FSlateLayoutTransform(SelectionBoxRect.GetTopLeft())),
		FAppStyle::GetBrush(TEXT("MarqueeSelection")));
}

void SCRRecoilUnitGraphWidget::AddUnitUnderCursor()
{
	UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();
	FScopedTransaction Transaction(NSLOCTEXT("SCRRecoilUnitGraphWidget", "AddUnitUnderCursor", "Add Unit Under Cursor"));
	CurrentRecoilUnitGraph->Modify();
	FVector2d CurrentMouseRecoilLocation = GraphCoordsToRecoilCoords(BackgroundWidget->PanelCoordToGraphCoord(CurrentMousePanelPosition));
	CurrentRecoilUnitGraph->AddUnit(CurrentMouseRecoilLocation);
}

void SCRRecoilUnitGraphWidget::AddUnit(FVector2d RecoilLocation)
{
	UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();
	FScopedTransaction Transaction(NSLOCTEXT("SCRRecoilUnitGraphWidget", "AddUnit", "Add Unit"));
	CurrentRecoilUnitGraph->Modify();
	CurrentRecoilUnitGraph->AddUnit(RecoilLocation);
}

void SCRRecoilUnitGraphWidget::RearrangeUnits()
{
}

void SCRRecoilUnitGraphWidget::CopySelectedUnits()
{
	FCRRecoilUnitSelection& CurrentUnitSelection = GetRecoilUnitSelection();
	UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();

	FCRRecoilUnitClipboardData CopiedData;
	
	TArray<FCRRecoilUnit*> CopiedUnits = CurrentUnitSelection.GetSelectedRecoilUnits(CurrentRecoilUnitGraph);
	for(auto& Unit : CopiedUnits)
	{
		CopiedData.RecoilUnitLocations.Add(Unit->Location);
	}
	FString ExportedString;
	FCRRecoilUnitClipboardData::StaticStruct()->ExportText(ExportedString, &CopiedData, nullptr, nullptr, PPF_None, nullptr);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedString);
}

void SCRRecoilUnitGraphWidget::PasteUnits()
{
	FCRRecoilUnitClipboardData CopiedData;
	FString CopiedString;
	FPlatformApplicationMisc::ClipboardPaste(CopiedString);
	if(!CopiedData.ImportFromString(CopiedString))
	{
		return;
	}
	UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();
	FVector2d CurrentMouseRecoilLocation = GraphCoordsToRecoilCoords(BackgroundWidget->PanelCoordToGraphCoord(CurrentMousePanelPosition));
	FScopedTransaction Transaction(NSLOCTEXT("SCRRecoilUnitGraphWidget", "PasteUnits", "Paste Units"));
	CurrentRecoilUnitGraph->Modify();
	for(auto& Location : CopiedData.RecoilUnitLocations)
	{
		CurrentRecoilUnitGraph->AddUnit(Location+CurrentMouseRecoilLocation);
	}
}

void SCRRecoilUnitGraphWidget::DeleteUnits()
{
	FCRRecoilUnitSelection& CurrentUnitSelection = GetRecoilUnitSelection();
	UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();
	FScopedTransaction Transaction(NSLOCTEXT("SCRRecoilUnitGraphWidget", "DeleteUnits", "Delete Units"));
	CurrentRecoilUnitGraph->Modify();
	TArray<int32> SelectedUnitIDs = CurrentUnitSelection.GetSelection();
	int32 UnitSelectionCount = SelectedUnitIDs.Num();
	for(int i = 0; i<UnitSelectionCount;i++)
	{
		CurrentRecoilUnitGraph->RemoveUnitByID(SelectedUnitIDs[i]);
	}
	CurrentUnitSelection.ClearSelection();
}

void SCRRecoilUnitGraphWidget::StartUnitScaling()
{
	TArray<FCRRecoilUnit*> SelectedUnits = GetRecoilUnitSelection().GetSelectedRecoilUnits(GetUnitGraph());
	FVector2D AverageLocation = FVector2d::ZeroVector;
	const int32 Num = SelectedUnits.Num();
	for(int i= 0; i<Num; i++)
	{
		AverageLocation += SelectedUnits[i]->Location;
	}
	AverageLocation /= Num;
	const FVector2d CurrentMouseRecoilPosition = GraphCoordsToRecoilCoords(BackgroundWidget->PanelCoordToGraphCoord(CurrentMousePanelPosition));
	const FVector2d MidRecoilLocation = (AverageLocation + CurrentMouseRecoilPosition) / 2.f;
	const FVector2D MidPanelLocation = BackgroundWidget->GraphCoordToPanelCoord(RecoilCoordsToGraphCoords(MidRecoilLocation));
	ScaleUnitsDrag = FCRUnitGraphScaleUnitsDelayedDrag(
		GetUnitGraph(),
		GetRecoilUnitSelection(),
		MidRecoilLocation,
		MidPanelLocation,
		EKeys::LeftMouseButton);
	ScaleUnitsDrag->NormalVectorSizePanel = (MidPanelLocation - CurrentMousePanelPosition).Size();
}

void SCRRecoilUnitGraphWidget::StopUnitScaling()
{
	if(ScaleUnitsDrag.IsSet())
	{
		ScaleUnitsDrag.Reset();
	}
}

UCRRecoilUnitGraph* SCRRecoilUnitGraphWidget::GetUnitGraph() const 
{
	return GetRecoilPattern()->GetUnitGraph();
}

UCRRecoilPattern* SCRRecoilUnitGraphWidget::GetRecoilPattern() const
{
	return RecoilPatternEditor->GetRecoilPattern();
}

FCRRecoilUnitSelection& SCRRecoilUnitGraphWidget::GetRecoilUnitSelection() const
{
	return RecoilPatternEditor->GetRecoilUnitSelection();
}

FVector2D SCRRecoilUnitGraphWidget::RecoilCoordsToGraphCoords(FVector2D RecoilCoords) const
{
	RecoilCoords.Y *= -1.f;
	RecoilCoords*= ScaleFromRecoilCoordsToGraphCoords;
	return RecoilCoords;
}

FVector2D SCRRecoilUnitGraphWidget::GraphCoordsToRecoilCoords(FVector2D GraphCoords) const
{
	GraphCoords.Y *= -1.f;
	GraphCoords /= ScaleFromRecoilCoordsToGraphCoords;
	return GraphCoords;
}

int32 SCRRecoilUnitGraphWidget::FindUnitByScreenLocation(const FVector2D& ScreenLocation, const FGeometry& MyGeometry) const
{
	const UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();
	const int32 UnitCount = CurrentRecoilUnitGraph->GetUnitCount();
	const FVector2D UnitDrawSize = CrystalRecoilEditor::UnitDrawSize;
	for(int i = 0; i<UnitCount; i++)
	{
		const FCRRecoilUnit& RecoilUnit = CurrentRecoilUnitGraph->GetUnitAt(i);
		const FVector2D RecoilUnitCenterGraphLocation = RecoilCoordsToGraphCoords(RecoilUnit.Location);
		const FVector2D RecoilUnitCenterPanelLocation = BackgroundWidget->GraphCoordToPanelCoord(RecoilUnitCenterGraphLocation);
		const FSlateRect UnitRect(
			RecoilUnitCenterPanelLocation - UnitDrawSize / 2.f,
			RecoilUnitCenterPanelLocation + UnitDrawSize / 2.f);
		if(UnitRect.ContainsPoint(ScreenLocation))
		{
			return RecoilUnit.ID;
		}
	}
	return INDEX_NONE;
}

void SCRRecoilUnitGraphWidget::SelectUnitsInPanelCoordsRect(FSlateRect& SelectionRect)
{
	const UCRRecoilUnitGraph* CurrentRecoilUnitGraph = GetUnitGraph();
	FCRRecoilUnitSelection& UnitSelection = GetRecoilUnitSelection();
	for(int i = 0;i< CurrentRecoilUnitGraph->GetUnitCount(); i++)
	{
		const FCRRecoilUnit& RecoilUnit = CurrentRecoilUnitGraph->GetUnitAt(i);
		const FVector2D RecoilUnitCenterGraphLocation = RecoilCoordsToGraphCoords(RecoilUnit.Location);
		const FVector2D RecoilUnitCenterPanelLocation = BackgroundWidget->GraphCoordToPanelCoord(RecoilUnitCenterGraphLocation);
		if(SelectionRect.ContainsPoint(RecoilUnitCenterPanelLocation))
		{
			UnitSelection.AddSelection(RecoilUnit.ID);
		}
		else
		{
			UnitSelection.RemoveSelection(RecoilUnit.ID);
		}
	}
}
