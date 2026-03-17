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

/**
 * 
 */

#define DELAY(time, block)\
{\
FTimerHandle TimerHandle;\
GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()block, time, false);\
}


UCLASS()
class BRAIDEDED_API AMyCharacter : public APaperCharacter, public IPoolableObject
{
	GENERATED_BODY()
	
public:
	AMyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Landed(const FHitResult& Hit) override;
	virtual void Death();

	virtual void SetActive_Implementation(bool Active) override;
	virtual bool IsActive_Implementation() override;
	virtual void Spawn_Implementation(FVector SpawnPoint, FRotator SpawnRotation) override;
	virtual void Despawn_Implementation() override;


	//FLIPBOOKS
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
protected:

	UFUNCTION()
	void OnSpikeCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void EnableCollision();
	virtual void DisableCollision();

	UPaperFlipbookComponent* FlipbookComponent;
	UCharacterMovementComponent* CharacterComponent;
	bool isGrounded;
	bool isVisible;
	bool isDead;
};
