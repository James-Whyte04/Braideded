// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterData.generated.h"

/**
 * 
 */

enum PlayerState
{
	Alive = 0,
	Dead = 1,
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY()

public:

	// Empty constructor
	FCharacterData() 
	{
		CharacterPosition = FVector(0.f, 0.f, 0.f);
		CharacterRotation = FRotator(0.f, 0.f, 0.f);
		Velocity = FVector(0.f, 0.f, 0.f);
		Flipbook = nullptr;
		FlipbookFrame = 0;
		MovementMode = EMovementMode::MOVE_None;
	}

	// Constuctor
	FCharacterData(FVector CharPos, FRotator CharRot, FVector Vel, UPaperFlipbook *Flipbook, int AnimState, TEnumAsByte<EMovementMode> Movement)
	{
		CharacterPosition = CharPos;
		CharacterRotation = CharRot;
		Velocity = Vel;
		this->Flipbook = Flipbook;
		FlipbookFrame = AnimState;
		MovementMode = Movement;
	}

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	FVector CharacterPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	FRotator CharacterRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	UPaperFlipbook* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	int FlipbookFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	TEnumAsByte<EMovementMode> MovementMode;


	// Operators
	FCharacterData& operator=(const FCharacterData& CD) 
	{
		CharacterPosition = CD.CharacterPosition;
		CharacterRotation = CD.CharacterRotation;
		Velocity = CD.Velocity;
		Flipbook = CD.Flipbook;
		FlipbookFrame = CD.FlipbookFrame;
		MovementMode = CD.MovementMode;

		return *this;
	}
};
