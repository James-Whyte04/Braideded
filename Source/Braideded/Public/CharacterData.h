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
		Frame = 0.f;
		MovementMode = EMovementMode::MOVE_None;
		IsVisible = false;
		IsDead = true;
	}

	// Constuctor
	FCharacterData(FVector CharPos, FRotator CharRot, FVector Vel, UPaperFlipbook *Flipbook, float Frame, TEnumAsByte<EMovementMode> Movement, bool IsVisible, bool IsDead = false)
	{
		CharacterPosition = CharPos;
		CharacterRotation = CharRot;
		Velocity = Vel;
		this->Flipbook = Flipbook;
		this->Frame = Frame;
		MovementMode = Movement;
		this->IsVisible = IsVisible;
		this->IsDead = IsDead;
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
	float Frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	TEnumAsByte<EMovementMode> MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	bool IsVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	bool IsDead;


	// Operators
	FCharacterData& operator=(const FCharacterData& CD) 
	{
		CharacterPosition = CD.CharacterPosition;
		CharacterRotation = CD.CharacterRotation;
		Velocity = CD.Velocity;
		Flipbook = CD.Flipbook;
		Frame = CD.Frame;
		MovementMode = CD.MovementMode;
		IsVisible = CD.IsVisible;
		IsDead = CD.IsDead;

		return *this;
	}
};
