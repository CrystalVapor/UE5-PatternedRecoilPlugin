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
 * Implement this interface on actor to provide recoil component and target controller for recoil system
 */
class CRYSTALRECOIL_API ICRRecoilInterface
{
	GENERATED_BODY()
public:
	/**
	 * Get the recoil component of the obj
	 * @return Recoil component of the obj
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, DisplayName = "Get Recoil Component")
	UCRRecoilComponent* K2_GetRecoilComponent();
	UCRRecoilComponent* K2_GetRecoilComponent_Implementation();

	/**
	 * Get the controller that should be used to apply recoil
	 * @return controller that should be used to apply recoil
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, DisplayName = "Get Target Controller")
	AController* K2_GetTargetController() const;
    AController* K2_GetTargetController_Implementation() const;
	
	/** 
	 * Start shooting, usually called when player press fire button
	 * Do not override if you don't know what you are doing
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Start Shooting", BlueprintNativeEvent)
	void K2_StartShooting();
    void K2_StartShooting_Implementation();
	
	/** 
	 * End shooting, usually called when player release fire button or after a certain time
	 * Do not override if you don't know what you are doing
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "End Shooting", BlueprintNativeEvent)
	void K2_EndShooting();
    void K2_EndShooting_Implementation();
	
	/** 
	 * Apply a shot to the recoil component, called when a shot is fired
	 * Do not override if you don't know what you are doing
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Apply Shot", BlueprintNativeEvent)
	void K2_ApplyShot();
	void K2_ApplyShot_Implementation();
	
	/**
	 * Do not call this directly in cpp, Call ICRRecoilInterface::Exec_K2_GetRecoilComponent(UObj*) instead
	 * need to be override if use function in cpp
	 * @return Recoil component of the obj
	 */
	virtual UCRRecoilComponent* GetRecoilComponent()
	{
		checkNoEntry()
		return nullptr;
	};

	/**
	 * Do not call this directly in cpp, Call ICRRecoilInterface::Exec_K2_GetRecoilComponent(UObj*) instead
	 * need to be override if use function in cpp
	 * @return controller that should be used to apply recoil
	 */
	virtual AController* GetTargetController() const
	{
		checkNoEntry()
		return nullptr;
	};
	
	virtual void StartShooting();

	virtual void EndShooting();

	virtual void ApplyShot();
};
