// Copyright CrystalVapor 2024, All rights reserved.

#include "Data/CRRecoilInterface.h"
#include "Components/CRRecoilComponent.h"

void ICRRecoilInterface::StartShooting()
{
	// Cast<UObject>(this) on an interface pointer produces a temporary with an uncertain lifetime.
	// Storing it in a named variable first ensures it stays alive for the duration of the Execute_ call.
	// Writing this as a one-liner causes the temporary to be destroyed too early, resulting in a crash.
	if (UCRRecoilComponent* RecoilComponent = Execute_K2_GetRecoilComponent(Cast<UObject>(this)))
	{
		RecoilComponent->StartShooting();
	}
}

void ICRRecoilInterface::EndShooting()
{
}

void ICRRecoilInterface::ApplyShot()
{
	if (UCRRecoilComponent* RecoilComponent = Execute_K2_GetRecoilComponent(Cast<UObject>(this)))
	{
		RecoilComponent->ApplyShot();
	}
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
