// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "Rewindable.h"
#include "IAction.h"
#include "Dilatable.h"
#include "MyCharacter.h"
#include "AC_Rewind.h"
#include "PlayerCharacter.generated.h"


class UInputAction;
class UInputMappingContext;
class UPaperFlipbookComponent;
class UPaperFlipbook;

// Description: Player character class, inherits from AMyCharacter,
// contains player mechanics such as movement and actions
UCLASS()
class BRAIDEDED_API APlayerCharacter : public AMyCharacter, public IRewindable, public IDilatable
{
	GENERATED_BODY()
	

public:
	APlayerCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Landed(const FHitResult& Hit) override;

	// Input setup
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Rewindable interface functions
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	// Time dilation interface functions
	virtual void IApplyDilationFactor_Implementation(float Factor);
	virtual void IClearTimeDilation_Implementation();

	virtual void Death() override;

protected:
	float dilationFactor = 1.f;

	// Camera components
	UPROPERTY (EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;


	// Actions, allows actions to select 
	// actions in editor
	UPROPERTY(EditAnywhere, Category = "Actions", meta = (MustImplement = "/Script/BRAIDEDED.Action"))
	TSubclassOf<UActorComponent> Action1;

	UPROPERTY(EditAnywhere, Category = "Actions", meta = (MustImplement = "/Script/BRAIDEDED.Action"))
	TSubclassOf<UActorComponent> Action2;


	// Input variables
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Ability1;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Ability2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Action1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Action2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_DeactivateAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_DeactivateAction2;


	// Flipbooks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action1Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action2Flipbook;


	// Action Pointers
	UActorComponent* ActionComponent1;
	UActorComponent* ActionComponent2;

	UObject* ActionObj1;
	UObject* ActionObj2;


	// Movement functions	
	void Walk(const FInputActionValue& Value);
	void StopWalk(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void Idle();

	// Action functions
	void ActivateAction1(const FInputActionValue& Value);
	void ActivateAction2(const FInputActionValue& Value);
	void DeactivateAction1(const FInputActionValue& Value);
	void DeactivateAction2(const FInputActionValue& Value);

	// Checks if player has rewind action equipped,
	// called on death
	int RewindCheck();

	// Adds action components selected in editor to the player character
	void SetActions();
	void ClearActions();
};
