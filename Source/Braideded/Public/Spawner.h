// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "PoolableObject.h"
#include "Rewindable.h"
#include "Dilatable.h"
#include "Spawner.generated.h"

#define DELAY(time, block)\
{\
FTimerHandle TimerHandle;\
GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()block, time, false);\
}

// Description: Spawns an actor of a class specified in the editor
// after a specified delay, impulse can be applied to characters
UCLASS()
class BRAIDEDED_API ASpawner : public AActor, public IRewindable, public IDilatable
{
	GENERATED_BODY()
	
public:	
	// Constructor
	ASpawner();

	// Rewind Interface functions
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	// Time Dilation Interface functions
	virtual void IApplyDilationFactor_Implementation(float Factor) override;
	virtual void IClearTimeDilation_Implementation() override;


protected:

	virtual void BeginPlay() override;


	// Standard components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperSpriteComponent* Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnProperties")
	float SpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnProperties")
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnProperties")
	FVector SpawnImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnProperties", meta = (MustImplement = "/Script/BRAIDEDED.PoolableObject"))
	TSubclassOf<AActor> ObjectToSpawn;

	TArray<AActor*> ObjectPool;

	FTimerHandle SpawnHandle;

	// Spawning functions
	// ResumeSpawn is called when exiting rewind,
	// this is required to start recurring spawning
	void Spawn();
	void ResumeSpawn();
	
	bool isActive;
	float SpawnTime;
};
