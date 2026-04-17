// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAction.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UAction : public UInterface
{
	GENERATED_BODY()
};

// Description: Interface for actions, when implemented to an actor component,
// allows it to be activated and deactivated by the player character, also allows it to record and clear data
class IAction : public IInterface
{
	GENERATED_BODY()

public:
	// Used for activating the action, Value can be used for variable dependent actions (e.g. Rewind: restore or rewind)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IActivate(float Value);

	// Used for deactivating the action, return to normal gameplay
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IDeactivate();

	// Used for recording necessary data for the action, called when the action is activated
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IRecord();

	// Used for clearing internal data from the action
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IClear();
};
