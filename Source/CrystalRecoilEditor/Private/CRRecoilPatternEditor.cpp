// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilPatternEditor.h"

#include "CRRecoilPattern.h"
#include "CRRecoilPatternEditorCommands.h"
#include "CRRecoilUnitGraph.h"
#include "IStructureDetailsView.h"
#include "SGridLineSpacingList.h"
#include "Widget/CRRecoilUnitGraphEditor.h"

const FName FCRRecoilPatternEditor::ToolkitFName(FName("CRRecoilPatternEditor"));
const FName FCRRecoilPatternEditor::UnitGraphTabFName(FName("CRRecoilPatternEditor_UnitGraph"));
const FName FCRRecoilPatternEditor::UnitGraphDetailsTabFName(FName("CRRecoilPatternEditor_UnitGraphDetails"));
const FName FCRRecoilPatternEditor::UnitDetailsTabFName(FName("CRRecoilPatternEditor_UnitDetails"));
const FName FCRRecoilPatternEditor::RecoilDetailsTabFName(FName("CRRecoilPatternEditor_RecoilDetails"));

void FCRRecoilUnitSelection::AddSelection(const int32 UnitID)
{
	SelectedUnits.AddUnique(UnitID);
	OnSelectionChanged.Broadcast();
}

void FCRRecoilUnitSelection::AddSelection(const TArray<int32>& UnitsArray)
{
	SelectedUnits.Append(UnitsArray);
	OnSelectionChanged.Broadcast();
}

void FCRRecoilUnitSelection::AddSelection(const TArray<FCRRecoilUnit>& RecoilUnitsArray)
{
	for(auto& RecoilUnit : RecoilUnitsArray){
		SelectedUnits.AddUnique(RecoilUnit.ID);
	}
	OnSelectionChanged.Broadcast();
}

void FCRRecoilUnitSelection::RemoveSelection(const int32 UnitID)
{
	SelectedUnits.Remove(UnitID);
	OnSelectionChanged.Broadcast();
}

void FCRRecoilUnitSelection::ClearSelection()
{
	SelectedUnits.Empty();
	OnSelectionChanged.Broadcast();
}

const TArray<int32>& FCRRecoilUnitSelection::GetSelection() const
{
	return SelectedUnits;
}

TArray<FCRRecoilUnit*> FCRRecoilUnitSelection::GetSelectedRecoilUnits(UCRRecoilUnitGraph* UnitGraph) const
{
	TArray<FCRRecoilUnit*> SelectedRecoilUnits;
	for(auto UnitID : SelectedUnits){
		SelectedRecoilUnits.Add(UnitGraph->GetUnitByID(UnitID));
	}
	return SelectedRecoilUnits;
}

bool FCRRecoilUnitSelection::IsEmpty() const
{
	return SelectedUnits.IsEmpty();
}

bool FCRRecoilUnitSelection::IsUnitSelected(const int32 UnitID) const
{
	return SelectedUnits.Contains(UnitID);
}

int32 FCRRecoilUnitSelection::GetNum() const
{
	return SelectedUnits.Num();
}

FCRRecoilPatternEditor::FCRRecoilPatternEditor()
{
	RecoilUnitSelection.OnSelectionChanged.AddRaw<FCRRecoilPatternEditor>(this, &FCRRecoilPatternEditor::OnSelectionChanged);
}

TSharedRef<FCRRecoilPatternEditor> FCRRecoilPatternEditor::CreateRecoilPatternEditor(
	EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UCRRecoilPattern* RecoilPattern)
{
	TSharedRef<FCRRecoilPatternEditor> RecoilPatternEditor = MakeShareable(new FCRRecoilPatternEditor());
	RecoilPatternEditor->InitializeEditor(Mode, InitToolkitHost, RecoilPattern);
	return RecoilPatternEditor;
}

FName FCRRecoilPatternEditor::GetToolkitFName() const
{
	return FName("CRRecoilPatternEditor");
}

FText FCRRecoilPatternEditor::GetBaseToolkitName() const
{
	return NSLOCTEXT("CrystalRecoil", "RecoilPatternEditorAppLabel", "RecoilPatternEditor");
}

FString FCRRecoilPatternEditor::GetWorldCentricTabPrefix() const
{
	return NSLOCTEXT("CrystalRecoil", "WorldCentricTabPrefix", "RecoilPattern").ToString();
}

FLinearColor FCRRecoilPatternEditor::GetWorldCentricTabColorScale() const
{
	return FColor::Cyan;
}

void FCRRecoilPatternEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(NSLOCTEXT("CRRecoilPatternEditor", "WorkspaceMenu_RecoilPatternEditor", "Recoil Pattern Editor"));

	InTabManager->RegisterTabSpawner(UnitGraphTabFName,
		FOnSpawnTab::CreateSP(this, &FCRRecoilPatternEditor::GetTab_UnitGraph))
		.SetDisplayName(NSLOCTEXT("CRRecoilPatternEditor", "UnitGraphTab", "Unit Graph"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(UnitGraphDetailsTabFName,
		FOnSpawnTab::CreateSP(this, &FCRRecoilPatternEditor::GetTab_UnitGraphDetails))
		.SetDisplayName(NSLOCTEXT("CRRecoilPatternEditor", "UnitGraphDetailsTab", "Unit Graph Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(UnitDetailsTabFName,
		FOnSpawnTab::CreateSP(this, &FCRRecoilPatternEditor::GetTab_UnitDetails))
		.SetDisplayName(NSLOCTEXT("CRRecoilPatternEditor", "UnitDetailsTab", "Unit Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(RecoilDetailsTabFName,
		FOnSpawnTab::CreateSP(this, &FCRRecoilPatternEditor::GetTab_RecoilDetails))
		.SetDisplayName(NSLOCTEXT("CRRecoilPatternEditor", "RecoilDetailsTab", "Recoil Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FCRRecoilPatternEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

UCRRecoilPattern* FCRRecoilPatternEditor::GetRecoilPattern() const
{
	return CastChecked<UCRRecoilPattern>(GetEditingObject());
}

UCRRecoilUnitGraph* FCRRecoilPatternEditor::GetRecoilUnitGraph() const
{
	UCRRecoilPattern* RecoilPattern = GetRecoilPattern();
	return RecoilPattern ? RecoilPattern->GetUnitGraph() : nullptr;
}

FCRRecoilUnitSelection& FCRRecoilPatternEditor::GetRecoilUnitSelection()
{
	return RecoilUnitSelection;
}

const FCRRecoilUnitSelection& FCRRecoilPatternEditor::GetRecoilUnitSelection() const
{
	return RecoilUnitSelection;
}

void FCRRecoilPatternEditor::MapCommands()
{
	const FCRRecoilPatternEditorCommands& Commands = FCRRecoilPatternEditorCommands::Get();
	TSharedRef<FUICommandList> EditorCommands = GetToolkitCommands();

	EditorCommands->MapAction(
		Commands.AddUnitUnderCursor,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_AddUnitUnderCursor),
		FCanExecuteAction::CreateLambda([]() { return true; }));
	
	EditorCommands->MapAction(
		Commands.RemoveUnit,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_RemoveUnit),
		FCanExecuteAction::CreateLambda([&]() { return !RecoilUnitSelection.IsEmpty(); }));
	
	EditorCommands->MapAction(
		Commands.SelectAll,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_SelectAll),
		FCanExecuteAction::CreateLambda([]() { return true; }));
	
	EditorCommands->MapAction(
		Commands.CopyUnits,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_CopyUnits),
		FCanExecuteAction::CreateLambda([&]() { return !RecoilUnitSelection.IsEmpty(); }));
	
	EditorCommands->MapAction(
		Commands.PasteUnits,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_PasteUnits),
		FCanExecuteAction::CreateLambda([]() { return true; }));
	
	EditorCommands->MapAction(
		Commands.UnitsSnapping,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_SwitchUnitSnapping),
		FCanExecuteAction::CreateLambda([]() { return true; }),
		FIsActionChecked::CreateLambda([&]() { return bEnableGridSnapping; })
		);

	EditorCommands->MapAction(
		Commands.AutoRearrangeUnits,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_SwitchAutoRearrangeUnits),
		FCanExecuteAction::CreateLambda([]() { return true; }),
		FIsActionChecked::CreateLambda([&]() { return bEnableAutoRearrangeUnits; })
		);

	EditorCommands->MapAction(
		Commands.UnitScaling,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_SwitchUnitScaling),
		FCanExecuteAction::CreateLambda([this]() { return !RecoilUnitSelection.IsEmpty(); }),
		FIsActionChecked::CreateLambda([&]() { return bEnableUnitScaling; })
		);

	EditorCommands->MapAction(
		Commands.ResetViewToOrigin,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_ResetViewToOrigin));

	EditorCommands->MapAction(
		Commands.ToggleShortcutsVisibility,
		FExecuteAction::CreateRaw(this, &FCRRecoilPatternEditor::Command_ToggleShortcutsVisibility),
		FCanExecuteAction::CreateLambda([]() { return true; }),
		FIsActionChecked::CreateLambda([&]() { return bShowShortcuts; })
	);
}

void FCRRecoilPatternEditor::ExtendToolBar()
{
	auto RegisterToolBarExtensions = [&](FToolBarBuilder& Builder)
	{
		const FCRRecoilPatternEditorCommands& Commands = FCRRecoilPatternEditorCommands::Get();
		Builder.PushCommandList(ToolkitCommands);
		Builder.BeginSection("RecoilPatternToolBarExtHook");

		Builder.AddToolBarButton(
			Commands.UnitScaling,
			"RecoilPatternToolBarExtHook",
			TAttribute<FText>(),
			TAttribute<FText>(),
			FSlateIcon());
		Builder.AddSeparator();
		Builder.AddToolBarButton(
			Commands.UnitsSnapping,
			"RecoilPatternToolBarExtHook",
			TAttribute<FText>(),
			TAttribute<FText>(),
			FSlateIcon("EditorStyle", "GenericCurveEditor.ToggleInputSnapping"));
		Builder.AddComboButton(
			FUIAction(),
			FOnGetContent::CreateSP(this, &FCRRecoilPatternEditor::GetMenuContent_UnitsSnapping),
			FText(),
			Commands.UnitsSnapping->GetDescription(),
			FSlateIcon());
		Builder.AddSeparator();
		Builder.AddToolBarButton(Commands.AutoRearrangeUnits);
		Builder.AddSeparator();
		Builder.AddToolBarButton(Commands.ResetViewToOrigin);

		Builder.EndSection();
		Builder.PopCommandList();
	};
	const TSharedPtr<FExtender> ToolBarExtender = MakeShared<FExtender>();
	ToolBarExtender->AddToolBarExtension("Asset",
		EExtensionHook::After,
		ToolkitCommands,
		FToolBarExtensionDelegate::CreateLambda(RegisterToolBarExtensions));
	AddToolbarExtender(ToolBarExtender);
}

void FCRRecoilPatternEditor::InitializeEditor(const EToolkitMode::Type Mode,
                                              const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InAsset)
{
	check(InAsset);
	UCRRecoilPattern* RecoilPattern = Cast<UCRRecoilPattern>(InAsset);
	check(RecoilPattern);
	UCRRecoilUnitGraph* RecoilUnitGraph = RecoilPattern->GetUnitGraph();
	check(RecoilUnitGraph);

	UWorld* World = RecoilUnitGraph->GetWorld();
	if( World && World->WorldType != EWorldType::EditorPreview)
	{
		// avoid too many unit editions' side effect for IDs.
		// rearrange only if we are not in PIE
		RecoilUnitGraph->RearrangeID();
	}

	// Map the commands in this editor's commandList and functions
	MapCommands();
	
	InitAssetEditor(Mode, InitToolkitHost, ToolkitFName, CreateEditorLayout(), true, true, InAsset);

	ExtendToolBar();
	
	RegenerateMenusAndToolbars();
}

TSharedRef<FTabManager::FLayout> FCRRecoilPatternEditor::CreateEditorLayout()
{
	return FTabManager::NewLayout(ToolkitFName)
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->AddTab(UnitGraphTabFName, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewSplitter()
						->SetOrientation(Orient_Horizontal)
						->Split
						(
							FTabManager::NewStack()
							->AddTab(UnitDetailsTabFName, ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
							->AddTab(UnitGraphDetailsTabFName, ETabState::OpenedTab)
						)
					)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(RecoilDetailsTabFName, ETabState::OpenedTab)
				)
			)
		);
}

void FCRRecoilPatternEditor::BuildTab_UnitGraph()
{
	UnitGraphWidget = SNew(SCRRecoilUnitGraphWidget)
		.RecoilPatternEditor(this);
}

void FCRRecoilPatternEditor::BuildTab_UnitGraphDetails()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	UnitGraphDetailsWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
}

void FCRRecoilPatternEditor::BuildTab_UnitDetails()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	UnitDetailsWidget = PropertyEditorModule.CreateStructureDetailView(DetailsViewArgs, FStructureDetailsViewArgs(), nullptr);
}

void FCRRecoilPatternEditor::BuildTab_RecoilDetails()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	RecoilDetailsWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
}

TSharedRef<SDockTab> FCRRecoilPatternEditor::GetTab_UnitGraph(const FSpawnTabArgs& Args)
{
	if(!UnitGraphWidget.IsValid()){
		BuildTab_UnitGraph();
	}

	UnitGraphWidget->SetObject(GetRecoilUnitGraph());
	
	return SNew(SDockTab)
		.Label(NSLOCTEXT("CRRecoilPatternEditor", "UnitGraphTab", "Unit Graph"))
		[
			UnitGraphWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FCRRecoilPatternEditor::GetTab_UnitGraphDetails(const FSpawnTabArgs& Args)
{
	if(!UnitGraphDetailsWidget.IsValid()){
		BuildTab_UnitGraphDetails();
	}

	UnitGraphDetailsWidget->SetObject(GetRecoilUnitGraph());
	
	return SNew(SDockTab)
		.Label(NSLOCTEXT("CRRecoilPatternEditor", "UnitGraphDetailsTab", "Unit Graph Details"))
		[
			UnitGraphDetailsWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FCRRecoilPatternEditor::GetTab_UnitDetails(const FSpawnTabArgs& Args)
{
	if(!UnitDetailsWidget.IsValid()){
		BuildTab_UnitDetails();
	}
	return SNew(SDockTab)
		.Label(NSLOCTEXT("CRRecoilPatternEditor", "UnitDetailsTab", "Unit Details"))
		[
			UnitDetailsWidget->GetWidget().ToSharedRef()
		];
}

TSharedRef<SDockTab> FCRRecoilPatternEditor::GetTab_RecoilDetails(const FSpawnTabArgs& Args)
{
	if(!RecoilDetailsWidget.IsValid()){
		BuildTab_RecoilDetails();
	}

	RecoilDetailsWidget->SetObject(GetRecoilPattern());
	
	return SNew(SDockTab)
		.Label(NSLOCTEXT("CRRecoilPatternEditor", "RecoilDetailsTab", "Recoil Details"))
		[
			RecoilDetailsWidget.ToSharedRef()
		];
}

void FCRRecoilPatternEditor::Command_AddUnitUnderCursor()
{
	UnitGraphWidget->AddUnitUnderCursor();
}

void FCRRecoilPatternEditor::Command_RemoveUnit()
{
	UnitGraphWidget->DeleteUnits();
}

void FCRRecoilPatternEditor::Command_SelectAll()
{
	UCRRecoilUnitGraph* RecoilUnitGraph = GetRecoilUnitGraph();
	check(RecoilUnitGraph);
	TArray<FCRRecoilUnit> RecoilUnits = RecoilUnitGraph->GetRawData();
	RecoilUnitSelection.ClearSelection();
	RecoilUnitSelection.AddSelection(RecoilUnits);
}

void FCRRecoilPatternEditor::Command_CopyUnits()
{
	UnitGraphWidget->CopySelectedUnits();
}

void FCRRecoilPatternEditor::Command_PasteUnits()
{
	UnitGraphWidget->PasteUnits();
}

void FCRRecoilPatternEditor::Command_SwitchUnitSnapping()
{
	bEnableGridSnapping = !bEnableGridSnapping;
}

void FCRRecoilPatternEditor::Command_SwitchAutoRearrangeUnits()
{
	bEnableAutoRearrangeUnits = !bEnableAutoRearrangeUnits;
}

void FCRRecoilPatternEditor::Command_SwitchUnitScaling()
{
	bEnableUnitScaling = !bEnableUnitScaling;
	if(bEnableUnitScaling)
	{
		UnitGraphWidget->StartUnitScaling();
	}else
	{
		UnitGraphWidget->StopUnitScaling();
	}
}

void FCRRecoilPatternEditor::Command_ResetViewToOrigin()
{
	UnitGraphWidget->SetViewPointToGraphOrigin();
}

void FCRRecoilPatternEditor::Command_ToggleShortcutsVisibility()
{
	bShowShortcuts = !bShowShortcuts;
}

TSharedRef<SWidget> FCRRecoilPatternEditor::GetMenuContent_UnitsSnapping()
{
	TArray<SGridLineSpacingList::FNamedValue> SpacingAmounts;
	auto MakeNamedValue = [](float Value) {
		return SGridLineSpacingList::FNamedValue(Value, FText::FromString(FString::Printf(TEXT("%f"), Value)), FText::FromString(TEXT("")));
	};
	SpacingAmounts.Add(MakeNamedValue(0.1f));
	SpacingAmounts.Add(MakeNamedValue(0.2f));
	SpacingAmounts.Add(MakeNamedValue(0.5f));
	SpacingAmounts.Add(MakeNamedValue(1.0f));
	SpacingAmounts.Add(MakeNamedValue(2.0f));

	return SNew(SGridLineSpacingList)
		.DropDownValues(SpacingAmounts)
		.MinDesiredValueWidth(60)
		.Value_Lambda([this]()->TOptional<float>{return GridSnappingValue;})
		.OnValueChanged_Lambda([this](TOptional<float> InNewGridSnappingValue){GridSnappingValue = InNewGridSnappingValue.Get(1.0f);})
		.HeaderText(NSLOCTEXT("CrystalRecoil", "GridSnapping", "Grid Snapping Value"));
}

void FCRRecoilPatternEditor::SelectRecoilUnit(const int32 UnitID)
{
}

void FCRRecoilPatternEditor::OnRecoilUnitRemoved(const int32 UnitID)
{
	if(RecoilUnitSelection.IsUnitSelected(UnitID)){
		RecoilUnitSelection.RemoveSelection(UnitID);
	}
}

void FCRRecoilPatternEditor::OnEditUnitFromDetailsPanel(const FPropertyChangedEvent& PropertyChangedEvent)
{
}

void FCRRecoilPatternEditor::OnSelectionChanged()
{
	if(RecoilUnitSelection.GetNum()==1)
	{
		int32 UnitID = RecoilUnitSelection.GetSelection()[0];
		UCRRecoilUnitGraph* RecoilUnitGraph = GetRecoilUnitGraph();
		check(RecoilUnitGraph);
		FCRRecoilUnit* RecoilUnit = RecoilUnitGraph->GetUnitByID(UnitID);
		if(RecoilUnit && UnitDetailsWidget.IsValid()){
			TSharedPtr<FStructOnScope> ScopedSelectedUnitDetails = MakeShared<FStructOnScope>(FCRRecoilUnit::StaticStruct(), reinterpret_cast<uint8*>(RecoilUnit));
			UnitDetailsWidget->SetStructureData(ScopedSelectedUnitDetails);
		}
		else
		{
			UnitDetailsWidget->SetStructureData(nullptr);
		}
	}

	if(RecoilUnitSelection.GetNum()<=1)
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		if(bEnableUnitScaling)
		{
			ToolkitCommands->ExecuteAction(FCRRecoilPatternEditorCommands::Get().UnitScaling.ToSharedRef());
		}
	}
}
