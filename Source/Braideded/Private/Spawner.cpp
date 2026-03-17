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
	
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectToSpawn, OutActors);

	if (OutActors.Num() == 0) return;

	for (AActor* actor : OutActors)
	{
		// Crash prevention
		ObjectPool.Add(actor);
		IPoolableObject::Execute_SetActive(actor, false);
	}
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Running Spawn Logic");
	DELAY(SpawnDelay,
		{ Spawn(); });

}

void ASpawner::Spawn()
{
	for (AActor* actor : ObjectPool)
	{
		if (!IPoolableObject::Execute_IsActive(actor))
		{
			IPoolableObject::Execute_Spawn(actor, SpawnPoint->GetComponentLocation(), SpawnRotation);
			break;
		}
		else
		{
			continue;
		}
	}

}
