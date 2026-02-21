// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "Rewindable.h"
#include "IAction.h"
#include "MyCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

class UInputAction;
class UInputMappingContext;
class UPaperFlipbookComponent;
class UPaperFlipbook;


UCLASS()
class BRAIDEDED_API APlayerCharacter : public AMyCharacter, public IRewindable
{
	GENERATED_BODY()
	

	//MOVEMENT SPEED
	//IMC + IA
	//active mechanics
public:
	APlayerCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Landed(const FHitResult& Hit) override;

	//INPUT
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* Ability1MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* Ability2MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* Ability3MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAction2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAction3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IDeactivateAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IDeactivateAction2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IDeactivateAction3;




	//FLIPBOOKS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action1Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action2Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action3Flipbook;


	//REWIND INTERFACE FUNCTIONS
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;
	virtual void IRewind_Implementation() override;
	virtual void IRollback_Implementation() override;

protected:

	//CAMERA COMPONENTS
	UPROPERTY (EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;


	//ACTION VARIABLES
	UPROPERTY(EditAnywhere, Category = "Actions", meta = (MustImplement = "/Script/BRAIDEDED.Action"))
	TSubclassOf<UActorComponent> Action1;

	UPROPERTY(EditAnywhere, Category = "Actions", meta = (MustImplement = "/Script/BRAIDEDED.Action"))
	TSubclassOf<UActorComponent> Action2;

	UPROPERTY(EditAnywhere, Category = "Actions", meta = (MustImplement = "/Script/BRAIDEDED.Action"))
	TSubclassOf<UActorComponent> Action3;

	UActorComponent* ActionComponent1;
	UActorComponent* ActionComponent2;
	UActorComponent* ActionComponent3;

	UObject* ActionObj1;
	UObject* ActionObj2;
	UObject* ActionObj3;

	//MOVEMENT FUNCTIONS	
	void Walk(const FInputActionValue& Value);
	void StopWalk(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	//ACTION FUNCTIONS
	void ActivateAction1(const FInputActionValue& Value);
	void ActivateAction2(const FInputActionValue& Value);
	void ActivateAction3(const FInputActionValue& Value);
	void FDeactivateAction1(const FInputActionValue& Value);
	void FDeactivateAction2(const FInputActionValue& Value);
	void FDeactivateAction3(const FInputActionValue& Value);

	void SetActions();
	void ClearActions();

	void Idle();
};
