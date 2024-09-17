#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Toolkits/IToolkitHost.h"

class CRYSTALRECOILEDITOR_API FCRAssetTypeActions_RecoilPattern: public FAssetTypeActions_Base
{
public:
	// AssetTypeActions_Base interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
	// End of AssetTypeActions_Base interface
};
