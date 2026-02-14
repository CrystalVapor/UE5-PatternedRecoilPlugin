#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCRAssetTypeActions_RecoilPattern;

class FCrystalRecoilEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	void RegisterAssetActions();

	void UnregisterAssetActions() const;

protected:
	TSharedPtr<FCRAssetTypeActions_RecoilPattern> RecoilPatternAssetAction;
};
