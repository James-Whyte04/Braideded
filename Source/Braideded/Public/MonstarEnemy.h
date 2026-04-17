// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Rewindable.h"
#include "Dilatable.h"
#include "MonstarEnemy.generated.h"

// Description: Basic enemy, walks back and forth
UCLASS()
class BRAIDEDED_API AMonstarEnemy : public AEnemyCharacter, public IRewindable, public IDilatable 
{
	GENERATED_BODY()
	
public:
	AMonstarEnemy();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Rewind interface functions
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	// Time Dilation interface functions
	virtual void IApplyDilationFactor_Implementation(float Factor) override;
	virtual void IClearTimeDilation_Implementation() override;

protected:
	bool canWalk;
};
