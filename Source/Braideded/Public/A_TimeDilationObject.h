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

UCLASS()
class BRAIDEDED_API AA_TimeDilationObject : public AActor, public IPoolableObject, public IRewindable
{
	GENERATED_BODY()
	
public:	
	AA_TimeDilationObject();

	virtual void SetActive_Implementation(bool Active);
	virtual bool IsActive_Implementation();
	
	void SetUpParameters(UAC_TimeDilation* TimeDilationAC, float Radius, float DilationFactor);

	//REWIND INTERFACE FUNCTIONS
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

protected:
	virtual void BeginPlay() override;

	//THESE VARIABLES WILL BE MOVED TO THE COMPONENT LATER, BUT FOR TESTING PURPOSES THEY ARE IN THE ACTOR FOR NOW
	//REMOVE BEGIN PLAY TO A FUNCTION CALLED ON SPAWN

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbook* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbookComponent* FlipbookComponent;

	USphereComponent* RadiusCollider;

	float MaxTimeDilationFactor;
	float TimeDilationRadius;

private:
	//function to create the formula for time dilation based on values set in the editor
	void CreateFormula();
	float CalculateTimeDilationFactor(AActor* actor);
	bool isActive;

	// Variables for the formula, (equation of a line)
	float m, c;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
