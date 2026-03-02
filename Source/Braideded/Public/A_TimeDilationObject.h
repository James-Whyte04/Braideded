// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PoolableObject.h"
#include "A_TimeDilationObject.generated.h"

class UAC_TimeDilation;

UCLASS()
class BRAIDEDED_API AA_TimeDilationObject : public AActor, public IPoolableObject
{
	GENERATED_BODY()
	
public:	
	AA_TimeDilationObject();

	virtual void SetActive_Implementation(bool IsActive);
	virtual bool IsActive_Implementation();
	
	void SetUpParameters(UAC_TimeDilation* TimeDilationAC, float Radius, float DilationFactor);

protected:
	virtual void BeginPlay() override;

	//THESE VARIABLES WILL BE MOVED TO THE COMPONENT LATER, BUT FOR TESTING PURPOSES THEY ARE IN THE ACTOR FOR NOW
	//REMOVE BEGIN PLAY TO A FUNCTION CALLED ON SPAWN

	USphereComponent* RadiusCollider;

	float MaxTimeDilationFactor = 0.5f;
	float TimeDilationRadius = 100.f;

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
