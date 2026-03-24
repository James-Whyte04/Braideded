// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Dilatable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UDilatable : public UInterface
{
	GENERATED_BODY()
};

class IDilatable : public IInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IApplyDilationFactor(float factor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IClearTimeDilation();
};
