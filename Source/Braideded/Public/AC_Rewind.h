// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAction.h"
#include "Rewindable.h"
#include "CharacterData.h"
#include "AC_Rewind.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class BRAIDEDED_API UAC_Rewind : public UActorComponent, public IAction
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Rewind();

	// Interface functions
	virtual void IActivate_Implementation(float Value) override;
	virtual void IDeactivate_Implementation() override;
	virtual void IRecord_Implementation() override;
	virtual void IClear_Implementation() override;
	virtual float ITimeScale_Implementation() override;

	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Variables
	float timeScale;
	int bufferSize;
	int currentBufferIndex;
	bool isRewinding;
	bool canRewind;

	// Array for each character in the scene, Queue for the actor's states
	TArray< TArray<FCharacterData> > ActorsStates;
	TArray< UObject* > RewindableActors;
};
