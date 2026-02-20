// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "Rewindable.h"
#include "IAction.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

class UInputAction;
class UInputMappingContext;
class UPaperFlipbookComponent;
class UPaperFlipbook;


UCLASS()
class BRAIDEDED_API APlayerCharacter : public APaperCharacter, public IRewindable
{
	GENERATED_BODY()
	

	//MOVEMENT SPEED
	//IMC + IA
	//active mechanics
public:
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;

	//Input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* RewindMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Action1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Action2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Action3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DeactivateAction1;




	//Flipbooks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* WalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* JumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action1Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action2Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action3Flipbook;


	//Functions

	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;
	virtual void IRewind_Implementation() override;
	virtual void IRollback_Implementation() override;


private:

	UPaperFlipbookComponent* FlipbookComponent;
	UCharacterMovementComponent* CharacterComponent;
	bool isGrounded;
	bool isRewinding;
	bool canRecord;

protected:

	UPROPERTY (EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UActorComponent* AbilityComponent1;
	UActorComponent* AbilityComponent2;
	UActorComponent* AbilityComponent3;

	UObject* Ability1;


	void IMove(const FInputActionValue& Value);
	void IStopMove(const FInputActionValue& Value);
	void IJump(const FInputActionValue& Value);
	void IStopJumping(const FInputActionValue& Value);
	void IAction1(const FInputActionValue& Value);
	void IAction2(const FInputActionValue& Value);
	void IAction3(const FInputActionValue& Value);
	void IDeactivateAction1(const FInputActionValue& Value);

	void Idle();
};
