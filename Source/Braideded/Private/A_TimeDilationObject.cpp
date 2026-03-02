// Fill out your copyright notice in the Description page of Project Settings.


#include "A_TimeDilationObject.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "AC_TimeDilation.h"

// Sets default values
AA_TimeDilationObject::AA_TimeDilationObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PrimaryActorTick.bCanEverTick = true;
	RadiusCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RadiusCollider"));
	RadiusCollider->SetupAttachment(RootComponent);
	RadiusCollider->SetSphereRadius(TimeDilationRadius);
}

// Called when the game starts or when spawned
void AA_TimeDilationObject::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("hello")));

	AA_TimeDilationObject::Execute_SetActive(this, false);
}

void AA_TimeDilationObject::SetUpParameters(UAC_TimeDilation* TimeDilationAC, float Radius, float DilationFactor)
{
	//Binds overlap events to the functions of the TimeDilation Component
	if (!TimeDilationAC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CAST TO TD FAILED")));
		return;
	}

	RadiusCollider->OnComponentBeginOverlap.AddDynamic(TimeDilationAC, &UAC_TimeDilation::AddToAffectedActors);
	RadiusCollider->OnComponentEndOverlap.AddDynamic(TimeDilationAC, &UAC_TimeDilation::RemoveFromAffectedActors);

	TimeDilationRadius = Radius;
	MaxTimeDilationFactor = DilationFactor;
}

// Called every frame
void AA_TimeDilationObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



/// <summary>
/// OBJECT POOLING FUNCTIONS
/// </summary>

void AA_TimeDilationObject::SetActive_Implementation(bool IsActive)
{
	this->isActive = IsActive;
	SetActorHiddenInGame(!IsActive);
}

bool AA_TimeDilationObject::IsActive_Implementation()
{
	return isActive;
}