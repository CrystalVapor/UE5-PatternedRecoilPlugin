// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilPatternEditorCommands.h"

#define LOCTEXT_NAMESPACE "CrystalRecoil"

void FCRRecoilPatternEditorCommands::RegisterCommands()
{
	UI_COMMAND(AddUnitUnderCursor,
		"Add Unit Under Cursor",
		"Add a new recoil unit under cursor",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::N, false, false, false, false));

	UI_COMMAND(RemoveUnit,
		"Remove Unit",
		"Remove the selected recoil unit",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::Platform_Delete));

	UI_COMMAND(SelectAll,
		"Select All",
		"Select all recoil units",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::A, false, true, false, false));

	UI_COMMAND(CopyUnits,
		"Copy Units",
		"Copy the selected recoil units",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::C, false, true, false, false));

	UI_COMMAND(PasteUnits,
		"Paste Units",
		"Paste the copied recoil units",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::V, false, true, false, false));

	UI_COMMAND(UnitsSnapping,
		"Unit Snapping",
		"Snap the units to the grid",
		EUserInterfaceActionType::ToggleButton,
		FInputChord(EKeys::S, true, false, false, false));

	UI_COMMAND(ResetViewToOrigin,
		"Reset View Point",
		"Reset View Point to Origin",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::O, false, false, false, false));

	UI_COMMAND(UnitScaling,
		"Unit Scaling",
		"Scale the selected units",
		EUserInterfaceActionType::ToggleButton,
		FInputChord(EKeys::S, false, false, false, false));

	UI_COMMAND(AutoRearrangeUnits,
		"Auto Rearrange Units",
		"Auto Rearrange Units",
		EUserInterfaceActionType::ToggleButton,
		FInputChord(EKeys::R, false, false, false, false));

	UI_COMMAND(ToggleShortcutsVisibility,
		"Toggle Shortcuts",
		"Show/Hide keyboard shortcuts",
		EUserInterfaceActionType::ToggleButton,
		FInputChord(EKeys::H, false, false, false, false));
}

#undef LOCTEXT_NAMESPACE
