// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PaperTileMapActor.h"
#include "PaperTileLayer.h"
#include "PaperTileSet.h"
#include "Projectile.h"

// Description: Base character class,
// contains basic movement and mechanics
// that are shared between the player and enemies


// Constructor
AMyCharacter::AMyCharacter()
{
	FlipbookComponent = GetSprite();
	CharacterComponent = GetCharacterMovement();
}




void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Binds OnSpikeCollision to OnComponentHit
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnSpikeCollision);
	GetCapsuleComponent()->BodyInstance.bLockYTranslation = true; // Lock Y Location
	GetCapsuleComponent()->BodyInstance.bLockXRotation = true;    // Lock X Rotation
	GetCapsuleComponent()->BodyInstance.bLockZRotation = true;	 // Lock Z Rotation
}




// Poolable Object Interface functions

void AMyCharacter::ISetActive_Implementation(bool Active)
{
	isVisible = Active;
	SetActorHiddenInGame(!isVisible);
}

bool AMyCharacter::IIsActive_Implementation()
{
	return isVisible;
}

void AMyCharacter::ISpawn_Implementation(FVector SpawnPoint, FRotator SpawnRotation)
{
	AMyCharacter::Execute_ISetActive(this, true);
	isDead = false;
	SetActorLocation(SpawnPoint);
	SetActorRotation(SpawnRotation);
	EnableCollision();
}

void AMyCharacter::IDespawn_Implementation()
{
	AMyCharacter::Execute_ISetActive(this, false);
	DisableCollision();
}



// Collision functions
void AMyCharacter::EnableCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMyCharacter::DisableCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyCharacter::Death()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MyCharacter.cpp: Dead");
	isDead = true;
}

bool AMyCharacter::IsDead()
{
	return isDead;
}




void AMyCharacter::OnSpikeCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Checks if the hit component is a TileMapActor
	APaperTileMapActor* Map = Cast<APaperTileMapActor>(Hit.GetActor());
	if (!Map)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MyCharacter.cpp: Collision is the map");

	//Gets the TileMapComponent of the TileMapActor
	UPaperTileMapComponent* TileMapComponent = Map->FindComponentByClass<UPaperTileMapComponent>();
	if (!TileMapComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CAST TO TMComp FAILED");
		return;
	}

	//Gets the coordinate of the point of impact
	FVector PointOfImpact = TileMapComponent->GetComponentTransform().InverseTransformPosition(Hit.ImpactPoint);

	UPaperTileMap* TileMap = TileMapComponent->TileMap;
	if (!TileMapComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MyCharacter.cpp: CAST TO TMap FAILED");
		return;
	}

	//Gets the Tile info of the Tile at the point of impact and sets the values of TileX and TileY 
	int TileX;
	int TileY;
	TileMap->GetTileCoordinatesFromLocalSpacePosition(PointOfImpact, TileX, TileY);

	//Gets the tile at position TileX and TileY
	FPaperTileInfo TileInfo = TileMapComponent->GetTile(TileX, TileY, 0);
	UPaperTileSet* TileSet = TileInfo.TileSet;
	if (!TileSet)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MyCharacter.cpp: CAST TO TSet FAILED");
		return;
	}

	//Gets the TileUserData to check if the hit Tile is a Spike 
	FName TileUserData = TileSet->GetTileUserData(TileInfo.GetTileIndex());

	if (TileUserData == "Spike")
	{
		//If Tile is a Spike, trigger death
		Death();
	}
	else
	{
		//Else ignore
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MyCharacter.cpp: Not spikes");
	}
}