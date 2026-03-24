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
	void ISetActive(bool Active);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IIsActive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IDespawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ISpawn(FVector SpawnPoint, FRotator SpawnRotation);
};