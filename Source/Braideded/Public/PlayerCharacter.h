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

/**
 * 
 */

class UInputAction;
class UInputMappingContext;
class UPaperFlipbookComponent;
class UPaperFlipbook;


UCLASS()
class BRAIDEDED_API APlayerCharacter : public AMyCharacter, public IRewindable, public IDilatable
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




	//FLIPBOOKS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action1Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Action2Flipbook;


	//REWIND INTERFACE FUNCTIONS
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	virtual void IApplyDilationFactor_Implementation(float Factor);
	virtual void IClearTimeDilation_Implementation();

	virtual void Death() override;

protected:
	float dilationFactor = 1.f;

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

	UActorComponent* ActionComponent1;
	UActorComponent* ActionComponent2;

	UObject* ActionObj1;
	UObject* ActionObj2;

	//MOVEMENT FUNCTIONS	
	void Walk(const FInputActionValue& Value);
	void StopWalk(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	//ACTION FUNCTIONS
	void ActivateAction1(const FInputActionValue& Value);
	void ActivateAction2(const FInputActionValue& Value);
	void DeactivateAction1(const FInputActionValue& Value);
	void DeactivateAction2(const FInputActionValue& Value);

	int RewindCheck();

	void SetActions();
	void ClearActions();

	void Idle();
};
