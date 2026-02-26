// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "A_TimeDilationObject.generated.h"

UCLASS()
class BRAIDEDED_API AA_TimeDilationObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AA_TimeDilationObject();

protected:
	virtual void BeginPlay() override;

	//THESE VARIABLES WILL BE MOVED TO THE COMPONENT LATER, BUT FOR TESTING PURPOSES THEY ARE IN THE ACTOR FOR NOW

	//max time dilation factor at the center of the object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float MaxTimeDilationFactor = 0.5f;

	//min and max radius for the time dilation effect, max is where the effect is strongest (max dilation applied), min is where its weakest (no dilation applied)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float TimeDilationMaxRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float TimeDilationMinRadius = 1000.f;

	USphereComponent* MinRadiusCollider;
	USphereComponent* MaxRadiusCollider;

private:
	//function to create the formula for time dilation based on values set in the editor
	void CreateFormula();
	float CalculateTimeDilationFactor(AActor* actor);

	// Variables for the formula, (equation of a line)
	float m, c;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
