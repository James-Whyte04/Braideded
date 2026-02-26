// Fill out your copyright notice in the Description page of Project Settings.


#include "A_TimeDilationObject.h"

// Sets default values
AA_TimeDilationObject::AA_TimeDilationObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MinRadiusCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MinRadiusCollider"));
	MinRadiusCollider->SetupAttachment(RootComponent);
	MaxRadiusCollider->SetSphereRadius(TimeDilationMaxRadius);

	MaxRadiusCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MaxRadiusCollider"));
	MaxRadiusCollider->SetupAttachment(RootComponent);
	MinRadiusCollider->SetSphereRadius(TimeDilationMinRadius);
}

// Called when the game starts or when spawned
void AA_TimeDilationObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_TimeDilationObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA_TimeDilationObject::CreateFormula()
{
	// Calculate the slope (m) and y-intercept (c) for a linear function that maps distance to time dilation factor
	m = (MaxTimeDilationFactor - 1.f) / (TimeDilationMinRadius - TimeDilationMaxRadius);
	c = (m * TimeDilationMaxRadius) - MaxTimeDilationFactor;
}

//if player is in MinDistance collider, run this formula, if in MaxDistance collider, return max dilation factor, if outside of MaxDistance collider, return 1 (no dilation)
float AA_TimeDilationObject::CalculateTimeDilationFactor(AActor* actor)
{
	// Calculate the time dilation factor based on the distance from the center of the object
	float TimeDilation = 1.f;
	float Distance = FVector::Dist(GetActorLocation(), actor->GetActorLocation());
	TimeDilation = (m * Distance) + c;
	
	return TimeDilation;
}
