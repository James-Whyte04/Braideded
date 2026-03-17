// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "PoolableObject.h"
#include "Spawner.generated.h"


#define DELAY(time, block)\
{\
FTimerHandle TimerHandle;\
GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()block, time, false);\
}



UCLASS()
class BRAIDEDED_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* Sprite;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(BlueprintReadWrite, Category = "SpawnProperties")
	float SpawnDelay;

	UPROPERTY(BlueprintReadWrite, Category = "SpawnProperties")
	FRotator SpawnRotation;

	UPROPERTY(BlueprintReadWrite, Category = "SpawnProperties", meta = (MustImplement = "/Script/BRAIDEDED.PoolableObject"))
	TSubclassOf<AActor> ObjectToSpawn;

	TArray<AActor*> ObjectPool;

	void Spawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
