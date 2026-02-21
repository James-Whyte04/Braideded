// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BRAIDEDED_API AMyCharacter : public APaperCharacter
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


	//FLIPBOOKS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* WalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* JumpFlipbook;

protected:

	UPaperFlipbookComponent* FlipbookComponent;
	UCharacterMovementComponent* CharacterComponent;
	bool isGrounded;
};
