// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IAction.h"
#include "Dilatable.h"
#include "PlayerCharacter.h"
#include "AC_TimeDilation.generated.h"

class AA_TimeDilationObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAIDEDED_API UAC_TimeDilation : public UActorComponent, public IAction
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_TimeDilation();

	virtual void IActivate_Implementation(float Value) override;
	virtual void IDeactivate_Implementation() override;
	virtual void IRecord_Implementation() override;
	virtual void IClear_Implementation() override;

	UFUNCTION() void AddToAffectedActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void RemoveFromAffectedActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CastToTDObject();

	AA_TimeDilationObject* TimeDilationActor;

	//max time dilation factor at the center of the object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float MaxTimeDilationFactor = 0.2f;

	//min and max radius for the time dilation effect, max is where the effect is strongest (max dilation applied), min is where its weakest (no dilation applied)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float TimeDilationRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float TimeDilationMaxRadius = 50.f;

	UFUNCTION(BlueprintCallable, Category = "Time Dilation")
	void CreateFormula();

	float m, c;

	TArray<AActor*> DilatableActors;
	TArray<AActor*> AffectedActors;
	TArray<AActor*> MaxAffectedActors;

	float CalculateTimeDilationFactor(AActor* actor);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
