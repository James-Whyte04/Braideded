// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "PaperCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnemyCharacter.generated.h"

// Description: Enemy character base class,
// inherits from AMyCharacter, contains basic mechanics
UCLASS()
class BRAIDEDED_API AEnemyCharacter : public AMyCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void Death() override;
	virtual void Move(float DeltaTime);

	void ChangeDirection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool canFall;

protected:
	UBoxComponent* HeadCollider;
	UBoxComponent* WallChecker;
	UBoxComponent* FloorChecker;

	// Collision functions
	virtual void EnableCollision() override;
	virtual void DisableCollision() override;

	UFUNCTION()
	void CheckWall(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() 
	void CheckFloor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void HeadHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle DeathHandle;

private:
	// Function to handle despawning the enemy after death
	// allows time for death animation to play before despawning
	void HandleDespawn();
	bool canChangeDirection;
};