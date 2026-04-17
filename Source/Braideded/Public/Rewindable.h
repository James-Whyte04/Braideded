// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterData.h"
#include "Rewindable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class URewindable : public UInterface
{
	GENERATED_BODY()
};

// Description: Interface for rewindable actors,
// when implemented to an actor, allows it to be rewound by the Rewind Action
class IRewindable : public IInterface
{
	GENERATED_BODY()

public:
	// Used for getting the snapshot of the actor's current state
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FCharacterData IGetCharacterSnapshot();

	// Used for setting the actor's state to a snapshot
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ISetCharacterSnapshot(FCharacterData CharData);

	// Called when the actor enters the rewind state,
	// ensure the actor is set up for rewinding (e.g. disable movement, set visibility, etc.)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IEnterRewindState();

	// Called when the actor exits the rewind state,
	// ensure the actor is set up to return to normal gameplay (e.g. enable movement, set visibility, etc.)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IExitRewindState(FCharacterData CharData);
};
