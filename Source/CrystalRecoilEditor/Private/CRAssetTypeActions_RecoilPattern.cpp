#include "CRAssetTypeActions_RecoilPattern.h"

#include "CRRecoilPattern.h"
#include "CRRecoilPatternEditor.h"

FText FCRAssetTypeActions_RecoilPattern::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_RecoilPattern", "Recoil Pattern");
}

FColor FCRAssetTypeActions_RecoilPattern::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FCRAssetTypeActions_RecoilPattern::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

UClass* FCRAssetTypeActions_RecoilPattern::GetSupportedClass() const
{
	return UCRRecoilPattern::StaticClass();
}

void FCRAssetTypeActions_RecoilPattern::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (auto Object : InObjects)
	{
		if (UCRRecoilPattern* RecoilPattern = Cast<UCRRecoilPattern>(Object))
		{
			FCRRecoilPatternEditor::CreateRecoilPatternEditor(Mode, EditWithinLevelEditor, RecoilPattern);
		}
	}
}
