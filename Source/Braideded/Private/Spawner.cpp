// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
	SetRootComponent(Sprite);

	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	isActive = true;

	// Find all objects in object pool of specified type and store them in an array
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectToSpawn, OutActors);

	if (OutActors.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Spawner.cpp: No objects of type in scene");
		return;
	}

	for (AActor* actor : OutActors)
	{
		// Crash prevention
		ObjectPool.Add(actor);
		IPoolableObject::Execute_IDespawn(actor);
	}

	// Setup Timer for spawn intervals
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &ASpawner::Spawn, SpawnDelay, true);
}




//SPAWNING FUNCTIONS
// General Spawn function
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

// This function is called when exiting rewind, this is required to start recurring spawning again via Timers
void ASpawner::ResumeSpawn()
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

	GetWorldTimerManager().SetTimer(SpawnHandle, this, &ASpawner::Spawn, SpawnDelay, true);
}




//REWIND INTERFACE FUNCTIONS
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




//TIME DILATION INTERFACE FUNCTIONS
void ASpawner::IApplyDilationFactor_Implementation(float Factor)
{
	this->CustomTimeDilation = Factor;
}

void ASpawner::IClearTimeDilation_Implementation()
{
	this->CustomTimeDilation = 1.f;
}