// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PoolableObject.h"
#include "Rewindable.h"
#include "Dilatable.h"
#include "Projectile.generated.h"

// Description: Basic projectile, moves with constant velocity,
// despawns on collision
UCLASS()
class BRAIDEDED_API AProjectile : public AActor, public IPoolableObject, public IRewindable, public IDilatable
{
	GENERATED_BODY()
	
public:	

	AProjectile();

	// Rewindable interface functions
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	//Time Dilation Interface functions
	virtual void IApplyDilationFactor_Implementation(float factor) override;
	virtual void IClearTimeDilation_Implementation() override;

	//Poolable Object interface functions
	virtual void ISetActive_Implementation(bool Active) override;
	virtual bool IIsActive_Implementation() override;
	virtual void ISpawn_Implementation(FVector SpawnPoint, FRotator SpawnRotation) override;
	virtual void IDespawn_Implementation() override;



protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void AddVelocity(float DeltaTime);

	// Collision function
	UFUNCTION()
	void OnProjectileCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Standard components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* ProjectileFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float Velocity;

	bool isVisible;
	bool isActive;
};
