// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Rewind.h"
#include "Rewindable.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Queue.h"
#include "Containers/CircularBuffer.h"


UAC_Rewind::UAC_Rewind()
{
	PrimaryComponentTick.bCanEverTick = true;
	bufferSize = 2000;
	currentBufferIndex = 1;
}




// Called when the game starts
void UAC_Rewind::BeginPlay()
{
	Super::BeginPlay();

	// Array of actors
	// Array of queue of their positions
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);;

	if (OutActors.Num() == 0) return;

	for (AActor* actor : OutActors) 
	{
		// Crash prevention

		if (actor->GetClass()->ImplementsInterface(URewindable::StaticClass()))
		{
			RewindableActors.Add(actor);
			UE_LOG(LogTemp, Warning, TEXT("ADDED"));
		}
	}

	ActorsStates.SetNum(RewindableActors.Num());

	for (int i = 0; i < RewindableActors.Num(); i++) 
	{
		ActorsStates[i].Reserve(bufferSize);
	}

	isRewinding = false;
}




void UAC_Rewind::IActivate_Implementation(float Value)
{
	// Set world time to 0
	// Restore positions of rewindable objects
	if (RewindableActors.Num() == 0) return;

	if (currentBufferIndex == 1 && Value < 0) canRewind = false;
	else if	(currentBufferIndex == ActorsStates[RewindableActors.Num() - 1].Num() - 1 && Value > 0) canRewind = false;
	else canRewind = true;

	if (!canRewind) return;
	const int32 Step = (Value > 0.1f) ? 1 : (Value < -0.1f ? -1 : 0);
	currentBufferIndex = currentBufferIndex + (1 * Step);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("index: %d"), currentBufferIndex)
	);

	for (int i = 0; i < RewindableActors.Num(); i++) 
	{
		if (!isRewinding)
		{
			IRewindable::Execute_IEnterRewindState(RewindableActors[i]);
		}

		FCharacterData Char = ActorsStates[i][ActorsStates[i].Num() - currentBufferIndex];
		IRewindable::Execute_ISetCharacterSnapshot(RewindableActors[i], Char);
	}

	isRewinding = true;
}




void UAC_Rewind::IDeactivate_Implementation()
{
	// Set world time to 1
	// Set objects in motion again

	if (RewindableActors.Num() == 0) return;

	for (int i = 0; i < RewindableActors.Num(); i++)
	{
		FCharacterData Char = ActorsStates[i][ActorsStates[i].Num() - currentBufferIndex];
		int Index = ActorsStates[i].Num() - currentBufferIndex;
		int Length = ActorsStates[i].Num();

		for (int j = Index; j < Length; j++) 
		{
			ActorsStates[i].RemoveAt(Index);
		}

		IRewindable::Execute_IExitRewindState(RewindableActors[i], Char);
	}

	currentBufferIndex = 1;
	isRewinding = false;
}




void UAC_Rewind::IRecord_Implementation()
{
	if (RewindableActors.Num() == 0) return;

	for (int i = 0; i < RewindableActors.Num(); i++) 
	{
		FCharacterData Char = IRewindable::Execute_IGetCharacterSnapshot(RewindableActors[i]);
		if (ActorsStates[i].Num() >= bufferSize) 
		{
			ActorsStates[i].RemoveAt(0);
		}
		ActorsStates[i].Add(Char);
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Char.CharacterPosition.ToString());
	}
}




void UAC_Rewind::IClear_Implementation()
{
	// Clear all elements from the heap
	// Call this when loading a new scene

	if (RewindableActors.Num() == 0) return;

	RewindableActors.Empty();
	
	for (int i = 0; i < ActorsStates.Num(); i++) 
	{
		ActorsStates[i].Empty();
	}

	ActorsStates.Empty();
}




float UAC_Rewind::ITimeScale_Implementation()
{
	if (isRewinding) 
	{
		timeScale = -1.f;
		return timeScale;
	}
	else
	{
		timeScale = 1.f;
		return timeScale;
	}
}
