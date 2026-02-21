// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

AMyCharacter::AMyCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	//WalkSpeed = 300.f;
	//SprintSpeed = 600.f;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}