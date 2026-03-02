// Copyright CrystalVapor 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Styling/AppStyle.h"

class FCRRecoilPatternEditorCommands : public TCommands<FCRRecoilPatternEditorCommands>
{
public:
	FCRRecoilPatternEditorCommands()
		: TCommands(FName("RecoilPatternEditorWidget"), FText::GetEmpty(), NAME_None, FAppStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> RemoveUnit;

	TSharedPtr<FUICommandInfo> SelectAll;

	TSharedPtr<FUICommandInfo> CopyUnits;

	TSharedPtr<FUICommandInfo> PasteUnits;

	TSharedPtr<FUICommandInfo> ToggleShortcutsVisibility;

	// Toolbar
	TSharedPtr<FUICommandInfo> UnitsSnapping;

	TSharedPtr<FUICommandInfo> ZoomViewToFit;

	TSharedPtr<FUICommandInfo> UnitScaling;

	TSharedPtr<FUICommandInfo> AutoRearrangeUnits;
};
