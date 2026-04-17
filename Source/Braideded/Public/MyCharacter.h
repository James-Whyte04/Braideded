// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PoolableObject.h"
#include "PaperTileMap.h"
#include "PaperTileMapComponent.h"
#include "MyCharacter.generated.h"

#define DELAY(time, block)\
{\
FTimerHandle TimerHandle;\
GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()block, time, false);\
}

// Description: Base character class,
// contains basic movement and mechanics
// that are shared between the player and enemies
UCLASS()
class BRAIDEDED_API AMyCharacter : public APaperCharacter, public IPoolableObject
{
	GENERATED_BODY()
	
public:
	AMyCharacter();

	virtual void BeginPlay() override;
	virtual void Death();
	bool IsDead();

	// Poolable Object Interface functions
	virtual void ISetActive_Implementation(bool Active) override;
	virtual bool IIsActive_Implementation() override;
	virtual void ISpawn_Implementation(FVector SpawnPoint, FRotator SpawnRotation) override;
	virtual void IDespawn_Implementation() override;

protected:

	UFUNCTION()
	void OnSpikeCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void EnableCollision();
	virtual void DisableCollision();

	// Movement variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	// Flipbooks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* WalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* JumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* FallFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* DeathFlipbook;

	UPaperFlipbookComponent* FlipbookComponent;
	UCharacterMovementComponent* CharacterComponent;
	bool isGrounded;
	bool isVisible;
	bool isDead;
};
