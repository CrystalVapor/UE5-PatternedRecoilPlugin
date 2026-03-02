// Copyright CrystalVapor 2026, All rights reserved.

#include "AssetTools/CRRecoilPatternFactory.h"
#include "Data/CRRecoilPattern.h"

UCRRecoilPatternFactory::UCRRecoilPatternFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UCRRecoilPattern::StaticClass();
}

UObject* UCRRecoilPatternFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UCRRecoilPattern::StaticClass()));
	return NewObject<UCRRecoilPattern>(InParent, InClass, InName, Flags);
}

FString UCRRecoilPatternFactory::GetDefaultNewAssetName() const
{
	return FString("RP_WeaponPattern");
}

FText UCRRecoilPatternFactory::GetToolTip() const
{
	return NSLOCTEXT("CrystalRecoil", "RecoilPatternFactoryToolTip", "Weapon Recoil Pattern Asset");
}
