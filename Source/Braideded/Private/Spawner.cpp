// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
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





void ASpawner::Spawn()
{
	if (!isActive) return;

	for (AActor* actor : ObjectPool)
	{
		if (!IPoolableObject::Execute_IIsActive(actor))
		{
			IPoolableObject::Execute_ISpawn(actor, SpawnPoint->GetComponentLocation(), SpawnRotation);
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
	if (!isActive) return;

	for (AActor* actor : ObjectPool)
	{
		if (!IPoolableObject::Execute_IIsActive(actor))
		{
			IPoolableObject::Execute_ISpawn(actor, SpawnPoint->GetComponentLocation(), SpawnRotation);
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