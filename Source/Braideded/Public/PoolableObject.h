// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableObject.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UPoolableObject : public UInterface
{
	GENERATED_BODY()
};

class IPoolableObject : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetActive(bool Active);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsActive();
};