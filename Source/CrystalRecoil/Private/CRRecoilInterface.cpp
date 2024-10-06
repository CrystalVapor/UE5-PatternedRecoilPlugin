// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilInterface.h"

#include "CRRecoilComponent.h"


void ICRRecoilInterface::StartShooting()
{
	// Strange... if you write these two line in the same line, it will not work.
	// split to a definition and a call will work....
	UCRRecoilComponent* RecoilComponent = Execute_K2_GetRecoilComponent(Cast<UObject>(this));
	RecoilComponent->StartNewRecoilSequence();
}

void ICRRecoilInterface::EndShooting()
{
	UCRRecoilComponent* RecoilComponent = Execute_K2_GetRecoilComponent(Cast<UObject>(this));
}

void ICRRecoilInterface::ApplyShot()
{
	UCRRecoilComponent* RecoilComponent = Execute_K2_GetRecoilComponent(Cast<UObject>(this));
	RecoilComponent->ApplyShot();
}

void ICRRecoilInterface::K2_StartShooting_Implementation()
{
	StartShooting();
}

void ICRRecoilInterface::K2_EndShooting_Implementation()
{
	EndShooting();
}

void ICRRecoilInterface::K2_ApplyShot_Implementation()
{
	ApplyShot();
}

UCRRecoilComponent* ICRRecoilInterface::K2_GetRecoilComponent_Implementation()
{
	return GetRecoilComponent();
}

AController* ICRRecoilInterface::K2_GetTargetController_Implementation() const
{
	return GetTargetController();
}
