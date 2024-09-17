// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
class SCRRecoilUnitGraphWidget;
class UCRRecoilUnitGraph;
class UCRRecoilPattern;

class FCRRecoilUnitSelection
{
public:
	void AddSelection(const int32 UnitID);
	void AddSelection(const TArray<int32>& UnitsArray);
	void AddSelection(const TArray<struct FCRRecoilUnit>& RecoilUnitsArray);

	void RemoveSelection(const int32 UnitID);
	void ClearSelection();

	const TArray<int32>& GetSelection() const;
	TArray<FCRRecoilUnit*> GetSelectedRecoilUnits(UCRRecoilUnitGraph* UnitGraph) const;

	bool IsEmpty() const;
	bool IsUnitSelected(const int32 UnitID) const;

	int32 GetNum() const;

	FSimpleMulticastDelegate OnSelectionChanged;
protected:
	TArray<int32> SelectedUnits;
};

/**
 * Editor toolkit for recoil pattern asset
 */
class CRYSTALRECOILEDITOR_API FCRRecoilPatternEditor : public FAssetEditorToolkit
{
public:
	FCRRecoilPatternEditor();

	static TSharedRef<FCRRecoilPatternEditor> CreateRecoilPatternEditor(EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost,
	                                                                    UCRRecoilPattern* RecoilPattern);
	
    virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	UCRRecoilPattern* GetRecoilPattern() const;
	UCRRecoilUnitGraph* GetRecoilUnitGraph() const;
	FCRRecoilUnitSelection& GetRecoilUnitSelection();
	const FCRRecoilUnitSelection& GetRecoilUnitSelection() const;

	void MapCommands();
	void ExtendToolBar();
	void InitializeEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InAsset);

	TSharedRef<FTabManager::FLayout> CreateEditorLayout();

	void BuildTab_UnitGraph();
	void BuildTab_UnitGraphDetails();
	void BuildTab_UnitDetails();
	void BuildTab_RecoilDetails();
	TSharedRef<SDockTab> GetTab_UnitGraph(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> GetTab_UnitGraphDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> GetTab_UnitDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> GetTab_RecoilDetails(const FSpawnTabArgs& Args);

	void Command_AddUnitUnderCursor();
	void Command_RemoveUnit();
	void Command_SelectAll();
	void Command_CopyUnits();
	void Command_PasteUnits();
	void Command_SwitchUnitSnapping();
	void Command_SwitchAutoRearrangeUnits();
	void Command_SwitchUnitScaling();
	void Command_ResetViewToOrigin();

	TSharedRef<SWidget> GetMenuContent_UnitsSnapping();

	void SelectRecoilUnit(const int32 UnitID);
	void OnRecoilUnitRemoved(const int32 UnitID);

	void OnEditUnitFromDetailsPanel(const FPropertyChangedEvent& PropertyChangedEvent);

	bool bEnableUnitScaling = false;
	bool bEnableAutoRearrangeUnits = true;
	bool bEnableGridSnapping = false;
	float GridSnappingValue = 0.1f;
protected:
	void OnSelectionChanged();
	
	static const FName ToolkitFName;
	static const FName UnitGraphTabFName;
	static const FName UnitGraphDetailsTabFName;
	static const FName UnitDetailsTabFName;
	static const FName RecoilDetailsTabFName;

	FCRRecoilUnitSelection RecoilUnitSelection;

	TSharedPtr<SCRRecoilUnitGraphWidget>	UnitGraphWidget;
	TSharedPtr<IDetailsView>				UnitGraphDetailsWidget;
	TSharedPtr<IStructureDetailsView>		UnitDetailsWidget;
	TSharedPtr<IDetailsView>				RecoilDetailsWidget;
};
