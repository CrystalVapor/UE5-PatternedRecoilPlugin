// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilInterface.h"

#include "CRRecoilComponent.h"


void ICRRecoilInterface::StartShooting()
{
	// Strange... if you write these two line in the same line, it will not work.
	// split to a definition and a call will work....
	UCRRecoilComponent* RecoilComponent = GetRecoilComponent();
	RecoilComponent->StartNewRecoilSequence();
}

void ICRRecoilInterface::EndShooting()
{
	UCRRecoilComponent* RecoilComponent = GetRecoilComponent();
}

void ICRRecoilInterface::ApplyShot()
{
	UCRRecoilComponent* RecoilComponent = GetRecoilComponent();
	RecoilComponent->ApplyShot();
}
