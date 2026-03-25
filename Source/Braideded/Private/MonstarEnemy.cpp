// Fill out your copyright notice in the Description page of Project Settings.


#include "MonstarEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

AMonstarEnemy::AMonstarEnemy()
{
	WalkSpeed = 200.f;

	GetCapsuleComponent()->SetCapsuleHalfHeight(45.f);
	GetCapsuleComponent()->SetCapsuleRadius(30.f);

	float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	HeadCollider->SetupAttachment(RootComponent);

	HeadCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HeadCollider->SetBoxExtent(FVector(Radius - 1.f, Radius - 1.f, 10.f));
	HeadCollider->SetRelativeLocation(FVector(0.f, 0.f, Height));

	FloorChecker->SetupAttachment(RootComponent);

	FloorChecker->SetBoxExtent(FVector(Radius, Radius, 10.f));
	FloorChecker->SetRelativeLocation(FVector((2.f * Radius), 0.f, -Height));

	WallChecker->SetupAttachment(RootComponent);

	WallChecker->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WallChecker->SetBoxExtent(FVector(20.f, Radius, Height));
	WallChecker->SetRelativeLocation(FVector((2.f * Radius), 0.f, 0.f));

	FlipbookComponent = GetSprite();
	CharacterComponent = GetCharacterMovement();

	CharacterComponent->MaxWalkSpeed = WalkSpeed;
	CharacterComponent->bOrientRotationToMovement = false;
}

void AMonstarEnemy::BeginPlay()
{
	Super::BeginPlay();
	canWalk = true;
}

void AMonstarEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canWalk) 
	{
		Move(DeltaTime);
	}
}




/// <summary>
/// REWIND INTERFACE FUNCTIONS
/// </summary>

FCharacterData AMonstarEnemy::IGetCharacterSnapshot_Implementation()
{
	float PlaybackTime = FlipbookComponent->GetPlaybackPosition();

	FCharacterData Char = FCharacterData(GetActorLocation(),
		GetActorRotation(),
		GetVelocity(),
		FlipbookComponent->GetFlipbook(),
		FlipbookComponent->GetFlipbook()->GetKeyFrameIndexAtTime(PlaybackTime),
		CharacterComponent->MovementMode,
		isVisible,
		isDead);
	return Char;
}

void AMonstarEnemy::ISetCharacterSnapshot_Implementation(FCharacterData CharData)
{
	// Set character pos, rot, flipbook, flipbook state
	SetActorLocation(CharData.CharacterPosition);
	SetActorRotation(CharData.CharacterRotation);
	FlipbookComponent->SetFlipbook(CharData.Flipbook);
	FlipbookComponent->SetPlaybackPosition(CharData.Frame, true);
	AMonstarEnemy::Execute_ISetActive(this, CharData.IsVisible);
	isDead = CharData.IsDead;
}

void AMonstarEnemy::IEnterRewindState_Implementation()
{
	DisableCollision();
	CharacterComponent->DisableMovement();
	FlipbookComponent->Stop();
	canWalk = false;

	if (GetWorldTimerManager().IsTimerActive(DeathHandle))
	{
		GetWorldTimerManager().ClearTimer(DeathHandle);
	}
}

void AMonstarEnemy::IExitRewindState_Implementation(FCharacterData CharData)
{
	canWalk = true;

	//Set position and rotation
	SetActorLocation(CharData.CharacterPosition);
	SetActorRotation(CharData.CharacterRotation);

	isDead = CharData.IsDead;

	//Set flipbook and frame
	FlipbookComponent->Play();

	if (isDead)
	{
		DisableCollision();
		Death();
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MonstarEnemy.cpp: dead");
		return;
	}
	else
	{
		EnableCollision();
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MonstarEnemy.cpp: alive and well")));
	}

	//Set movement mode
	CharacterComponent->SetMovementMode(CharData.MovementMode);

	//Set velocity
	CharacterComponent->Velocity = CharData.Velocity;

	switch (CharacterComponent->MovementMode)
	{
	case MOVE_Falling:
		FlipbookComponent->SetFlipbook(JumpFlipbook);
		FlipbookComponent->SetPlaybackPositionInFrames(CharData.Frame, false);
		break;
	case MOVE_Walking:
		if (CharacterComponent->GetCurrentAcceleration().X == 0)
		{
			FlipbookComponent->SetFlipbook(IdleFlipbook);
		}
		else
		{
			FlipbookComponent->SetFlipbook(WalkFlipbook);
		}

		FlipbookComponent->SetPlaybackPositionInFrames(CharData.Frame, false);
		break;
	default:
		FlipbookComponent->SetFlipbook(IdleFlipbook);
		FlipbookComponent->SetPlaybackPositionInFrames(CharData.Frame, false);
		break;
	}

	AMonstarEnemy::Execute_ISetActive(this, CharData.IsVisible);
}



/// <summary>
/// TIME DILATION INTERFACE FUNCTIONS
/// </summary>

void AMonstarEnemy::IApplyDilationFactor_Implementation(float Factor)
{
	this->CustomTimeDilation = Factor;
}

void AMonstarEnemy::IClearTimeDilation_Implementation()
{
	this->CustomTimeDilation = 1.f;
}