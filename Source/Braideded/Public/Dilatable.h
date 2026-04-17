// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Dilatable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UDilatable : public UInterface
{
	GENERATED_BODY()
};

// Description: Interface for dilatable actors,
// allows them to be affected by time dilation
class IDilatable : public IInterface
{
	GENERATED_BODY()

public:

	// Used for applying a time dilation factor to the actor,
	// factor is between 0 and 1, where 0 is completely frozen and 1 is normal time
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IApplyDilationFactor(float factor);

	// Used for clearing any time dilation effects on the actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IClearTimeDilation();
};
