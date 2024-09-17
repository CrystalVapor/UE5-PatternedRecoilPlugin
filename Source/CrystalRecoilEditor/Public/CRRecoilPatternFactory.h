// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"

#include "CRRecoilPatternFactory.generated.h"

/**
 * 
 */
UCLASS(HideCategories = Object)
class CRYSTALRECOILEDITOR_API UCRRecoilPatternFactory: public UFactory
{
	GENERATED_BODY()
public:
	UCRRecoilPatternFactory();

	// Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual FText GetToolTip() const override;
	// End UFactory Interface
};
