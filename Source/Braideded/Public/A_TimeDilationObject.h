// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PoolableObject.h"
#include "Rewindable.h"
#include "CharacterData.h"
#include "A_TimeDilationObject.generated.h"

class UAC_TimeDilation;

// Description: The actor that is spawned in scene
// when the time dilation action is activated,
// sends the actors that enter its radius to the 
// Time Dilation Component to time dilation to them
UCLASS()
class BRAIDEDED_API AA_TimeDilationObject : public AActor, public IPoolableObject, public IRewindable
{
	GENERATED_BODY()
	
public:	
	// Constructor
	AA_TimeDilationObject();

	// IPoolableObject interface functions
	virtual void SetActive_Implementation(bool Active);
	virtual bool IsActive_Implementation();

	// Rewind Interface functions
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	// Function to set up the parameters of the time dilation object,
	// called by the Time Dilation Component on Begin Play
	void SetUpParameters(UAC_TimeDilation* TimeDilationAC, float Radius, float DilationFactor);

protected:
	virtual void BeginPlay() override;

	// Standard components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbookComponent* FlipbookComponent;

	USphereComponent* RadiusCollider;

private:

	bool isActive;
};
