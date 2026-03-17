// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Rewindable.h"
#include "Dilatable.h"
#include "MonstarEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BRAIDEDED_API AMonstarEnemy : public AEnemyCharacter, public IRewindable, public IDilatable 
{
	GENERATED_BODY()
	
public:
	AMonstarEnemy();
	virtual void Tick(float DeltaTime) override;

	//REWIND INTERFACE FUNCTIONS
	virtual FCharacterData IGetCharacterSnapshot_Implementation() override;
	virtual void ISetCharacterSnapshot_Implementation(FCharacterData CharData) override;
	virtual void IEnterRewindState_Implementation() override;
	virtual void IExitRewindState_Implementation(FCharacterData CharData) override;

	virtual void ApplyDilationFactor_Implementation(float factor);
	virtual void ClearTimeDilation_Implementation();
};
