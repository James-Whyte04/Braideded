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
		IPoolableObject::Execute_Despawn(actor);
	}

	// Setup Timer for spawn intervals
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpawner::Spawn, SpawnDelay, true);
}

void ASpawner::Spawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Spawner.cpp: Running");

	for (AActor* actor : ObjectPool)
	{
		if (!IPoolableObject::Execute_IsActive(actor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Spawner.cpp: Spawned");
			IPoolableObject::Execute_Spawn(actor, SpawnPoint->GetComponentLocation(), SpawnRotation);
			break;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Spawner.cpp: Failed")));
			continue;
		}
	}

}
