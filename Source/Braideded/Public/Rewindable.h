// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterData.h"
#include "Rewindable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class URewindable : public UInterface
{
	GENERATED_BODY()
};

class IRewindable : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FCharacterData IGetCharacterSnapshot();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ISetCharacterSnapshot(FCharacterData CharData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IEnterRewindState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IExitRewindState(FCharacterData CharData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IRewind();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IRollback();
};
