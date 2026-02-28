// Copyright CrystalVapor 2024, All rights reserved.

#include "AssetTools/CRRecoilPatternFactory.h"
#include "Data/CRRecoilPattern.h"
#include "Data/CRRecoilUnitGraph.h"

UCRRecoilPatternFactory::UCRRecoilPatternFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UCRRecoilPattern::StaticClass();
}

UObject* UCRRecoilPatternFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	const auto RecoilPattern = NewObject<UCRRecoilPattern>(InParent, InClass, InName, Flags);
	const auto UnitGraph = RecoilPattern->GetUnitGraph();
	check(UnitGraph != nullptr);
	UnitGraph->AddUnit(FVector2f(0.2f, 1.f));
	UnitGraph->AddUnit(FVector2f(0.45f, 2.55f));
	UnitGraph->AddUnit(FVector2f(1.2f, 4.05f));
	return RecoilPattern;
}

FText UCRRecoilPatternFactory::GetToolTip() const
{
	return NSLOCTEXT("CrystalRecoil", "RecoilPatternFactoryToolTip", "Optimized Recoil Pattern for shooting");
}
