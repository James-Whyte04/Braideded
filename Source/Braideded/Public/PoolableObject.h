// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableObject.generated.h"


UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UPoolableObject : public UInterface
{
	GENERATED_BODY()
};

// Description: Interface for poolable objects,
// when implemented to an actor, allows it to be pooled by the Object Pooler
class IPoolableObject : public IInterface
{
	GENERATED_BODY()

public:
	// Used for setting the active state of the object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ISetActive(bool Active);

	// Used for checking if the object is active
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IIsActive();

	// Used for despawning the object, called by the Object Pooler when the object is returned to the pool
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IDespawn();

	// Used for spawning the object, called by the Object Pooler when the object is taken from the pool
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ISpawn(FVector SpawnPoint, FRotator SpawnRotation);
};