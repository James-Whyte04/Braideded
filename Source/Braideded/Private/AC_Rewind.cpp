// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Rewind.h"
#include "Rewindable.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Queue.h"
#include "Containers/CircularBuffer.h"
#include "PlayerCharacter.h"

// Description: Rewind action, attached to the
// player character as actor component in editor

// Constructor
UAC_Rewind::UAC_Rewind()
{
	PrimaryComponentTick.bCanEverTick = true;
	bufferSize = 2000;
	currentBufferIndex = 1;
}




void UAC_Rewind::BeginPlay()
{
	Super::BeginPlay();

	// Gets all rewindable actors in the scene
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);

	if (OutActors.Num() == 0) return;

	for (AActor* actor : OutActors) 
	{
		if (actor->GetClass()->ImplementsInterface(URewindable::StaticClass()))
		{
			// Adds rewindable actors to the RewindableActors array
			RewindableActors.Add(actor);
		}
	}

	ActorsStates.SetNum(RewindableActors.Num());

	for (int i = 0; i < RewindableActors.Num(); i++) 
	{
		// Reserves buffer size for each actor's snapshots
		ActorsStates[i].Reserve(bufferSize);
	}

	isRewinding = false;
	canRewind = true;
}

void UAC_Rewind::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	IClear_Implementation();
}


void UAC_Rewind::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UAC_Rewind::Execute_IRecord(this);
}




// Action interface functions

void UAC_Rewind::IActivate_Implementation(float Value)
{
	// Set world time to 0
	// Restore positions of rewindable objects
	if (RewindableActors.Num() == 0) return;

	// Crash prevention for out of bounds index
	if (currentBufferIndex == 1 && Value < 0) canRewind = false;
	else if	(currentBufferIndex == ActorsStates[RewindableActors.Num() - 1].Num() - 1 && Value > 0) canRewind = false;
	else canRewind = true;

	if (!canRewind) return;

	// Disable tick while rewinding
	if (PrimaryComponentTick.bCanEverTick) SetComponentTickEnabled(false);

	const int32 Step = (Value > 0.1f) ? 1 : (Value < -0.1f ? -1 : 0);
	currentBufferIndex = currentBufferIndex + (1 * Step);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AC_Rewind.cpp: index: %d"), currentBufferIndex)
	);

	for (int i = 0; i < RewindableActors.Num(); i++) 
	{
		// Crash prevention for out of bounds index
		if (!IsValid(RewindableActors[i]))
		{
			RemoveActor(i);
			return;
		}

		// Call enter rewind state on actors on first activation
		if (!isRewinding)
		{
			IRewindable::Execute_IEnterRewindState(RewindableActors[i]);
		}

		// Set the snapshot of the actor to the snapshot at the current buffer index
		FCharacterData Char = ActorsStates[i][ActorsStates[i].Num() - currentBufferIndex];
		IRewindable::Execute_ISetCharacterSnapshot(RewindableActors[i], Char);
	}

	isRewinding = true;
}

void UAC_Rewind::IDeactivate_Implementation()
{
	// Prevent player exiting rewind state if they are dead
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

	if (Player && Player->IsDead()) return;

	if (RewindableActors.Num() == 0) return;

	for (int i = 0; i < RewindableActors.Num(); i++)
	{
		FCharacterData Char = ActorsStates[i][ActorsStates[i].Num() - currentBufferIndex];
		int Index = ActorsStates[i].Num() - currentBufferIndex;
		int Length = ActorsStates[i].Num();

		for (int j = Index; j < Length; j++) 
		{
			// Remove all snapshots that are ahead of the current buffer index
			ActorsStates[i].RemoveAt(Index);
		}

		IRewindable::Execute_IExitRewindState(RewindableActors[i], Char);
	}

	// Reset rewind properties
	currentBufferIndex = 1;
	isRewinding = false;
	SetComponentTickEnabled(true);
}

void UAC_Rewind::IRecord_Implementation()
{
	if (RewindableActors.Num() == 0) return;

	for (int i = 0; i < RewindableActors.Num(); i++) 
	{
		if (!IsValid(RewindableActors[i]))
		{
			RemoveActor(i);
			return;
		}

		FCharacterData Char = IRewindable::Execute_IGetCharacterSnapshot(RewindableActors[i]);
		if (ActorsStates[i].Num() >= bufferSize) 
		{
			ActorsStates[i].RemoveAt(0);
		}
		ActorsStates[i].Add(Char);
	}
}

void UAC_Rewind::IClear_Implementation()
{
	// Clear all elements 
	if (RewindableActors.Num() == 0) return;

	RewindableActors.Empty();
	
	for (int i = 0; i < ActorsStates.Num(); i++) 
	{
		ActorsStates[i].Empty();
	}

	ActorsStates.Empty();
}




void UAC_Rewind::RemoveActor(int Index)
{
	RewindableActors.RemoveAt(Index);
	ActorsStates[Index].Empty();
	ActorsStates.RemoveAt(Index);
}