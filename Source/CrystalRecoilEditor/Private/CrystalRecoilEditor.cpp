#include "CrystalRecoilEditor.h"
#include "AssetTools/CRAssetTypeActions_RecoilPattern.h"
#include "Editor/CRRecoilPatternEditorCommands.h"

#define LOCTEXT_NAMESPACE "CrystalRecoil"

void FCrystalRecoilEditorModule::StartupModule()
{
	FCRRecoilPatternEditorCommands::Register();
	RegisterAssetActions();
}

void FCrystalRecoilEditorModule::ShutdownModule()
{
	UnregisterAssetActions();
	FCRRecoilPatternEditorCommands::Unregister();
}

void FCrystalRecoilEditorModule::RegisterAssetActions()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RecoilPatternAssetAction = MakeShareable(new FCRAssetTypeActions_RecoilPattern());
	AssetTools.RegisterAssetTypeActions(RecoilPatternAssetAction.ToSharedRef());
}

void FCrystalRecoilEditorModule::UnregisterAssetActions() const
{
	if (RecoilPatternAssetAction.IsValid())
	{
		const FAssetToolsModule* AssetTools = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
		if (!AssetTools)
		{
			return;
		}
		IAssetTools& AssetToolsInterface = AssetTools->Get();
		AssetToolsInterface.UnregisterAssetTypeActions(RecoilPatternAssetAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCrystalRecoilEditorModule, CrystalRecoilEditor)
