// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Styling/AppStyle.h"

/**
 * 
 */
class FCRRecoilPatternEditorCommands: public TCommands<FCRRecoilPatternEditorCommands>
{
public:
	FCRRecoilPatternEditorCommands():TCommands<FCRRecoilPatternEditorCommands>("RecoilPatternEditorWidget", FText(), "", FAppStyle::GetAppStyleSetName()){}
	virtual void RegisterCommands() override;
	
	TSharedPtr<FUICommandInfo> AddUnitUnderCursor;
	TSharedPtr<FUICommandInfo> RemoveUnit;
	TSharedPtr<FUICommandInfo> SelectAll;
	TSharedPtr<FUICommandInfo> CopyUnits;
	TSharedPtr<FUICommandInfo> PasteUnits;
	//Toolbar
	TSharedPtr<FUICommandInfo> UnitsSnapping;
	TSharedPtr<FUICommandInfo> ResetViewToOrigin;
	TSharedPtr<FUICommandInfo> UnitScaling;
	TSharedPtr<FUICommandInfo> AutoRearrangeUnits;
	TSharedPtr<FUICommandInfo> ToggleShortcutsVisibility;
};
