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



UCLASS()
class BRAIDEDED_API ASpawner : public AActor, public IRewindable, public IDilatable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();


	//REWIND INTERFACE FUNCTIONS
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	//TIME DILATION INTERFACE FUNCTIONS
	virtual void IApplyDilationFactor_Implementation(float Factor) override;
	virtual void IClearTimeDilation_Implementation() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
//	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnProperties", meta = (MustImplement = "/Script/BRAIDEDED.PoolableObject"))
	TSubclassOf<AActor> ObjectToSpawn;

	TArray<AActor*> ObjectPool;

	FTimerHandle SpawnHandle;

	void Spawn();
	void ResumeSpawn();
	
	bool isActive;
	float SpawnTime;
};
