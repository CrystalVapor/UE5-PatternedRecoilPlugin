// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CRRecoilInterface.generated.h"

class UCRRecoilComponent;

UINTERFACE(Blueprintable)
class UCRRecoilInterface : public UInterface
{
	GENERATED_BODY()
};

/**
* Interface for actors that support the recoil system.
* Implement this interface to provide recoil component access and specify which controller receives recoil effects.
*/
class CRYSTALRECOIL_API ICRRecoilInterface
{
	GENERATED_BODY()
public:
	/**
	* Gets the recoil component attached to this actor.
	* Blueprint implementable version.
	* @return The actor's recoil component
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, DisplayName = "Get Recoil Component")
	UCRRecoilComponent* K2_GetRecoilComponent();
	UCRRecoilComponent* K2_GetRecoilComponent_Implementation();

	/**
	* Gets the controller that will receive recoil effects (camera kick).
	* Blueprint implementable version.
	* @return The target controller for recoil application
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, DisplayName = "Get Target Controller")
	AController* K2_GetTargetController() const;
	AController* K2_GetTargetController_Implementation() const;

	/**
	* Initiates a new recoil sequence (burst fire).
	* Call this when the player presses the fire button.
	* Blueprint implementable version - override only if you need custom behavior.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Start Shooting", BlueprintNativeEvent)
	void K2_StartShooting();
	void K2_StartShooting_Implementation();

	/**
	* Ends the current recoil sequence.
	* Call this when the player releases the fire button or after a timeout.
	* Blueprint implementable version - override if you need custom behavior. Does nothing by default.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "End Shooting", BlueprintNativeEvent)
	void K2_EndShooting();
	void K2_EndShooting_Implementation();

	/**
	* Applies recoil for a single shot.
	* Call this each time a bullet is fired.
	* Blueprint implementable version - override only if you need custom behavior.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Apply Shot", BlueprintNativeEvent)
	void K2_ApplyShot();
	void K2_ApplyShot_Implementation();

	/**
	* C++ version of GetRecoilComponent.
	* Override this in C++ implementations instead of calling K2_ functions directly.
	* For execution from C++, use ICRRecoilInterface::Execute_GetRecoilComponent(Object) instead.
	* @return The actor's recoil component
	*/
	virtual UCRRecoilComponent* GetRecoilComponent()
	{
		checkNoEntry()
		return nullptr;
	};

	/**
	* C++ version of GetTargetController.
	* Override this in C++ implementations instead of calling K2_ functions directly.
	* For execution from C++, use ICRRecoilInterface::Execute_GetTargetController(Object) instead.
	* @return The target controller for recoil application
	*/
	virtual AController* GetTargetController() const
	{
		checkNoEntry()
		return nullptr;
	};

	/**
	* C++ version of StartShooting.
	* Override this in C++ implementations for custom shooting start logic.
	*/
	virtual void StartShooting();

	/**
	* C++ version of EndShooting.
	* Does nothing by default — override in C++ implementations if needed.
	*
	* Example use cases:
	* - Start burst cooldown tracking
	* - Trigger weapon heat dissipation
	* - Play fire-end audio/VFX
	*/
	virtual void EndShooting();

	/**
	* C++ version of ApplyShot.
	* Override this in C++ implementations for custom shot application logic.
	*/
	virtual void ApplyShot();
};
