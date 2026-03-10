// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PaperTileMapActor.h"
#include "PaperTileLayer.h"
#include "PaperTileSet.h"

AMyCharacter::AMyCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	//WalkSpeed = 300.f;
	//SprintSpeed = 600.f;
	FlipbookComponent = GetSprite();
	CharacterComponent = GetCharacterMovement();
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnSpikeCollision);
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

void AMyCharacter::SetActive_Implementation(bool Active)
{
	isActive = Active;
	SetActorHiddenInGame(!Active);
}

bool AMyCharacter::IsActive_Implementation()
{
	return isActive;
}

void AMyCharacter::OnSpikeCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Checks if the hit component is a TileMapActor
	APaperTileMapActor* Map = Cast<APaperTileMapActor>(Hit.GetActor());
	if (!Map)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed");
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Collision is the map")));

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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CAST TO TMap FAILED");
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CAST TO TSet FAILED");
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Not spikes");
	}
}

void AMyCharacter::Death()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Dead");
}
