// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAction.h"
#include "Dilatable.h"
#include "PlayerCharacter.h"
#include "AC_TimeDilation.generated.h"

class AA_TimeDilationObject;

// Description: Time Dilation action, attached to the player character, 
// Time Dilation Actor must be in scene for the action to work 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAIDEDED_API UAC_TimeDilation : public UActorComponent, public IAction
{
	GENERATED_BODY()

public:	

	UAC_TimeDilation();

	// Action interface functions
	virtual void IActivate_Implementation(float Value) override;
	virtual void IDeactivate_Implementation() override;
	virtual void IRecord_Implementation() override;
	virtual void IClear_Implementation() override;

	// Functions for adding and removing actors from the arrays of affected actors,
	// called by the Time Dilation Actor
	UFUNCTION() void AddToAffectedActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void RemoveFromAffectedActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:

	virtual void BeginPlay() override;

	// Function to cast the Time Dilation Actor in scene
	void CastToTDObject();

	// Reference to the Time Dilation Actor in scene
	AA_TimeDilationObject* TimeDilationActor;

	// Max time dilation factor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float MaxTimeDilationFactor = 0.2f;

	// Min and max radius for the time dilation effect,
	// max is where the effect is strongest (max dilation applied),
	// min is where its weakest (no dilation applied)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float TimeDilationRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float TimeDilationMaxRadius = 150.f;

	// Function to create the formula for calculating the time dilation factor,
	// based on the distance from the center of the object,
	// y = mx + c, where y is the time dilation factor,
	// m is the rate of factor increase (gradient),
	// x is the distance from the center of the object,
	UFUNCTION(BlueprintCallable, Category = "Time Dilation")
	void CreateFormula();

	// Variables for the formula
	float m, c;

	// Array of actors in the scene that can be affected by the time dilation effect
	TArray<AActor*> DilatableActors;

	// Array of actors currently affected by the time dilation effect,
	// updated by the Time Dilation Actor
	TArray<AActor*> AffectedActors;

	float CalculateTimeDilationFactor(AActor* actor);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
