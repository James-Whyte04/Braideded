// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_TimeDilation.h"
#include "Kismet/GameplayStatics.h"
#include "A_TimeDilationObject.h"

// Sets default values for this component's properties
UAC_TimeDilation::UAC_TimeDilation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_TimeDilation::BeginPlay()
{
	Super::BeginPlay();
	CreateFormula();
	CastToTDObject();

	// ...
	
}

void UAC_TimeDilation::CreateFormula()
{
	// Create the formula for time dilation based on values set in the editor
	// The formula is a linear equation of the form y = mx + c, where y is the time dilation factor, x is the distance from the center of the object, m is the slope of the line, and c is the y-intercept
	m = (1.f - MaxTimeDilationFactor) / (TimeDilationRadius - TimeDilationMaxRadius);
	c = 1.f - (m * TimeDilationRadius);
}

void UAC_TimeDilation::CastToTDObject() 
{
	//Sets the parameters of the TimeDilation Object
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AA_TimeDilationObject::StaticClass());
	TimeDilationActor = Cast<AA_TimeDilationObject>(actor);

	if (TimeDilationActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Casted to TDObejct")));
		TimeDilationActor->SetUpParameters(this, TimeDilationRadius, MaxTimeDilationFactor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Cast to TDObejct FAILED, add TimeDilationObject to scene")));
	}
}

// Called every frame
void UAC_TimeDilation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Record time dilation values for actors in the scene
	if (!TimeDilationActor) return;
	UAC_TimeDilation::Execute_IRecord(this);
}

void UAC_TimeDilation::IActivate_Implementation(float Value)
{
	//set time dilation actor to active
	if (!TimeDilationActor) return;

	AA_TimeDilationObject::Execute_SetActive(TimeDilationActor, true);
	TimeDilationActor->SetActorLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
}

void UAC_TimeDilation::IDeactivate_Implementation()
{
	if (!TimeDilationActor) return;

	//clear the time dilation of all affected actors
	if (AffectedActors.Num() > 0)
	{
		for (AActor* actor : AffectedActors)
		{
			if (actor->GetClass()->ImplementsInterface(UDilatable::StaticClass()))
			{
				IDilatable::Execute_ClearTimeDilation(actor);
			}
		}
	}

	//set time dilation actor to inactive
	AA_TimeDilationObject::Execute_SetActive(TimeDilationActor, false);
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
			IDilatable::Execute_ApplyDilationFactor(actor, TimeDilationFactor);
			FString FloatStr = FString::SanitizeFloat(TimeDilationFactor);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FloatStr);
		}
	}
}

void UAC_TimeDilation::IClear_Implementation()
{
	// Clear recorded time dilation values
	for (AActor* actor : AffectedActors) 
	{
		if (actor->GetClass()->ImplementsInterface(UDilatable::StaticClass())) 
		{
			IDilatable::Execute_ClearTimeDilation(actor);
		}
	}

	AffectedActors.Empty();
	DilatableActors.Empty();
}

float UAC_TimeDilation::CalculateTimeDilationFactor(AActor* actor)
{
	// Calculate the time dilation factor based on the distance from the center of the object

	float TimeDilationFactor = 1.f;
	float Distance = FVector::Dist(TimeDilationActor->GetActorLocation(), actor->GetActorLocation());
	FString Dist = FString::SanitizeFloat(Distance);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Dist);

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
		AffectedActors.Add(OtherActor);
	}
}

void UAC_TimeDilation::RemoveFromAffectedActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDilatable::StaticClass()) && AffectedActors.Contains(OtherActor))
	{
		AffectedActors.Remove(OtherActor);
	}
}