// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Constructor
ASpawner::ASpawner()
{
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
	SetRootComponent(Sprite);

	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(RootComponent);
}




void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	isActive = true;

	// Find all objects in object pool of selected class in scene
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectToSpawn, OutActors);

	if (OutActors.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Spawner.cpp: No objects of type in scene");
		return;
	}

	for (AActor* actor : OutActors)
	{
		// Adds all actors of selected class in scene
		// to the object pool array and despawns them
		ObjectPool.Add(actor);
		IPoolableObject::Execute_IDespawn(actor);
	}

	// Setup Timer for spawn intervals
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &ASpawner::Spawn, SpawnDelay, true);
}




// Spawning functions

void ASpawner::Spawn()
{
	if (!isActive) return;

	for (AActor* actor : ObjectPool)
	{
		if (!IPoolableObject::Execute_IIsActive(actor))
		{
			// Spawn actor at spawn point
			IPoolableObject::Execute_ISpawn(actor, SpawnPoint->GetComponentLocation(), SpawnRotation);

			// If is character, add impulse
			AMyCharacter* Character = Cast<AMyCharacter>(actor);
			if (Character) 
			{
				Character->LaunchCharacter(SpawnImpulse, false, false);
			}

			break;
		}
		else
		{
			continue;
		}
	}
}

void ASpawner::ResumeSpawn()
{
	Spawn();

	// Setup Timer for recurring spawns
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &ASpawner::Spawn, SpawnDelay, true);
}




// Rewind Interface functions

FCharacterData ASpawner::IGetCharacterSnapshot_Implementation()
{
	FCharacterData Character = FCharacterData();

	// Only stores the remaining time of the timer in the Frame float
	Character.Frame = GetWorldTimerManager().GetTimerRemaining(SpawnHandle);
	return Character;
}

void ASpawner::ISetCharacterSnapshot_Implementation(FCharacterData CharData)
{
	SpawnTime = CharData.Frame;
	return;
}

void ASpawner::IEnterRewindState_Implementation()
{
	isActive = false;
	GetWorldTimerManager().ClearTimer(SpawnHandle);
}

void ASpawner::IExitRewindState_Implementation(FCharacterData CharData)
{
	isActive = true;
	SpawnTime = CharData.Frame;

	// Calls the ResumeSpawn function
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &ASpawner::ResumeSpawn, SpawnTime, false);
}




// Time Dilation Interface functions
void ASpawner::IApplyDilationFactor_Implementation(float Factor)
{
	this->CustomTimeDilation = Factor;
}

void ASpawner::IClearTimeDilation_Implementation()
{
	this->CustomTimeDilation = 1.f;
}