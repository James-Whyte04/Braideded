// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAction.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UAction : public UInterface
{
	GENERATED_BODY()
};

class IAction : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IActivate(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IDeactivate();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IRecord();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IClear();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float ITimeScale();
};
