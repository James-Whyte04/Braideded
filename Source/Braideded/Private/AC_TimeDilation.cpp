// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_TimeDilation.h"
#include "Kismet/GameplayStatics.h"
#include "A_TimeDilationObject.h"

// Description: Time Dilation action, attached to the player character, 
// Time Dilation Actor must be in scene for the action to work 


// Constructor
UAC_TimeDilation::UAC_TimeDilation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}




void UAC_TimeDilation::BeginPlay()
{
	Super::BeginPlay();
	CreateFormula();
	CastToTDObject();	
}

void UAC_TimeDilation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Record time dilation values for actors in the scene
	if (!TimeDilationActor) return;
	UAC_TimeDilation::Execute_IRecord(this);
}



// Calculates the constants for the formula
// y = mx + c, based on parameters set
void UAC_TimeDilation::CreateFormula()
{
	m = (1.f - MaxTimeDilationFactor) / (TimeDilationRadius - TimeDilationMaxRadius);
	c = 1.f - (m * TimeDilationRadius);
}

void UAC_TimeDilation::CastToTDObject() 
{
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AA_TimeDilationObject::StaticClass());
	TimeDilationActor = Cast<AA_TimeDilationObject>(actor);

	if (TimeDilationActor)
	{
		//Sets the parameters of the Time Dilation Object
		TimeDilationActor->SetUpParameters(this, TimeDilationRadius, MaxTimeDilationFactor);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AC_TimeDilation: Casted to TimeDilationObject")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AC_TimeDilation: Cast to TimeDilationObject FAILED, add TimeDilationObject to scene")));
	}
}




// Action interface functions

void UAC_TimeDilation::IActivate_Implementation(float Value)
{
	// Set time dilation actor to active
	if (!TimeDilationActor) return;

	AA_TimeDilationObject::Execute_ISetActive(TimeDilationActor, true);
	TimeDilationActor->SetActorLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
}

void UAC_TimeDilation::IDeactivate_Implementation()
{
	if (!TimeDilationActor) return;

	// Clear the time dilation of all affected actors
	if (AffectedActors.Num() > 0)
	{
		for (AActor* actor : AffectedActors)
		{
			if (actor->GetClass()->ImplementsInterface(UDilatable::StaticClass()))
			{
				IDilatable::Execute_IClearTimeDilation(actor);
			}
		}
	}

	// Set time dilation actor to inactive
	AA_TimeDilationObject::Execute_ISetActive(TimeDilationActor, false);
	TimeDilationActor->SetActorLocation(FVector(0.f, 0.f, 0.f));
}

void UAC_TimeDilation::IRecord_Implementation()
{
	// Record time dilation values for actors in the scene
	if (AffectedActors.Num() == 0) return;

	for(AActor* actor : AffectedActors) 
	{
		if (actor->GetClass()->ImplementsInterface(UDilatable::StaticClass())) 
		{
			float TimeDilationFactor = CalculateTimeDilationFactor(actor);
			IDilatable::Execute_IApplyDilationFactor(actor, TimeDilationFactor);
		//	FString FloatStr = FString::SanitizeFloat(TimeDilationFactor);
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FloatStr);
		}
	}
}

void UAC_TimeDilation::IClear_Implementation()
{
	// Clear recorded time dilation values for each actor
	for (AActor* actor : AffectedActors) 
	{
		if (actor->GetClass()->ImplementsInterface(UDilatable::StaticClass())) 
		{
			IDilatable::Execute_IClearTimeDilation(actor);
		}
	}

	// Clear arrays
	AffectedActors.Empty();
	DilatableActors.Empty();
}




float UAC_TimeDilation::CalculateTimeDilationFactor(AActor* actor)
{
	// Calculate the time dilation factor based on the distance from the center of the object

	float TimeDilationFactor = 1.f;
	float Distance = FVector::Dist(TimeDilationActor->GetActorLocation(), actor->GetActorLocation());

	TimeDilationFactor = (m * Distance) + c;

	if (TimeDilationFactor < MaxTimeDilationFactor) 
	{
		return MaxTimeDilationFactor;
	}
	else if (TimeDilationFactor > 1.f)
	{
		return 1.f;
	}
	return TimeDilationFactor;
}

void UAC_TimeDilation::AddToAffectedActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDilatable::StaticClass()) && !AffectedActors.Contains(OtherActor))
	{
		// Adds actors that enter the radius of the Time Dilation Object to the AffectedActors array
		AffectedActors.Add(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Actor is in zone"));
	}
}

void UAC_TimeDilation::RemoveFromAffectedActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDilatable::StaticClass()) && AffectedActors.Contains(OtherActor))
	{
		// Removes actors that leave the radius of the Time Dilation Object from the AffectedActors array
		AffectedActors.Remove(OtherActor);
		IDilatable::Execute_IClearTimeDilation(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Actor has left zone"));
	}
}