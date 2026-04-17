// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.h"
#include "PaperTileMapActor.h"
#include "Projectile.h"
#include "Spawner.h"
#include "GameFramework/CharacterMovementComponent.h"

// Description: Enemy character base class,
// inherits from AMyCharacter, contains basic mechanics


// Constructor

AEnemyCharacter::AEnemyCharacter()
{
	float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	HeadCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollider"));
	HeadCollider->SetupAttachment(RootComponent);

	HeadCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HeadCollider->SetBoxExtent(FVector(Radius - 1.f, Radius - 1.f, 10.f));
	HeadCollider->SetRelativeLocation(FVector(0.f, 0.f, Height));

	FloorChecker = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorChecker"));
	FloorChecker->SetupAttachment(RootComponent);

	FloorChecker->SetBoxExtent(FVector(Radius, Radius, 10.f));
	FloorChecker->SetRelativeLocation(FVector((2.f * Radius), 0.f, -Height));

	WallChecker = CreateDefaultSubobject<UBoxComponent>(TEXT("WallChecker"));
	WallChecker->SetupAttachment(RootComponent);

	WallChecker->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WallChecker->SetBoxExtent(FVector(20.f, Radius, Height - 2.f));
	WallChecker->SetRelativeLocation(FVector((2.f * Radius), 0.f, 0.f));

	FlipbookComponent = GetSprite();
	CharacterComponent = GetCharacterMovement();
}




void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	canFall = true;
	canChangeDirection = true;
	isDead = false;
#
	if (canFall)
	{
		FloorChecker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		FloorChecker->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	// Binds collision functions to collision events
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyCharacter::BodyHit);
	HeadCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::HeadHit);
	FloorChecker->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CheckFloor);
	WallChecker->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CheckWall);
}




// Base death function

void AEnemyCharacter::Death()
{
	isDead = true;
	FlipbookComponent->SetFlipbook(DeathFlipbook);
	DisableCollision();
	GetCharacterMovement()->Velocity.Z = 0.f;
	LaunchCharacter(FVector(0.f, 0.f, 100.f), false, false);

	// Sets timer to despawn after death animation plays
	GetWorldTimerManager().SetTimer(DeathHandle, this, &AEnemyCharacter::HandleDespawn, 1.f, false);
}

void AEnemyCharacter::HandleDespawn()
{
	IPoolableObject::Execute_IDespawn(this);
}




// Base movement functions

void AEnemyCharacter::Move(float DeltaTime)
{
	if (!isVisible || isDead)
	{
		return;
	}

	// Add velocity
	AddMovementInput(GetActorForwardVector(), WalkSpeed * DeltaTime);

	// Set flipbook
	if (CharacterComponent->IsFalling())
	{
		if (CharacterComponent->Velocity.Z < 0.f)
		{
			FlipbookComponent->SetFlipbook(FallFlipbook);
		}
		else
		{
			FlipbookComponent->SetFlipbook(JumpFlipbook);
		}
	}
	else
	{
		FlipbookComponent->SetFlipbook(WalkFlipbook);
	}
}

void AEnemyCharacter::ChangeDirection()
{
	canChangeDirection = false;

	// Flip character rotation
	FRotator Rotation = GetActorRotation();
	FTransform NewTransform = FTransform(FRotator(Rotation.Pitch, Rotation.Yaw - 180.f, Rotation.Roll),
		GetActorLocation(),
		GetActorScale());

	SetActorTransform(NewTransform);

	// Crash prevention: set timer to prevent multiple direction changes in one frame
	GetWorldTimerManager().SetTimerForNextTick([this]()
		{ canChangeDirection = true;} );
}




// Base collision functions

void AEnemyCharacter::EnableCollision()
{
	Super::EnableCollision();

	HeadCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (canFall)
	{
		// Disables collision on floor checker, enabling falling
		FloorChecker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		// Enables collision on floor checker, preventing falling
		FloorChecker->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	CharacterComponent->SetMovementMode(MOVE_Walking);
}

void AEnemyCharacter::DisableCollision()
{
	Super::DisableCollision();
	
	HeadCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FloorChecker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacter::CheckWall(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EnemyCharacter.cpp: Triggered Wall Check");

	if (!canChangeDirection) return;

	if (Cast<APaperTileMapActor>(OtherActor))
	{
		// Changes direction if the hit component is a TileMapActor
		ChangeDirection();
		return;
	}

	else if (Cast<ASpawner>(OtherActor))
	{
		// Changes direction if the hit component is a Spawner
		ChangeDirection();
		return;
	}

	else if (Cast<AEnemyCharacter>(OtherActor) && OtherActor != this)
	{
		// Changes direction if the hit component is another enemy character
		ChangeDirection();
		return;
	}
}

void AEnemyCharacter::CheckFloor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EnemyCharacter.cpp: Triggered Floor Check");
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr && !canFall)
	{
		// Changes direction if the hit component is not
		// a player character and the enemy cannot fall
		ChangeDirection();
	}
}

void AEnemyCharacter::HeadHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EnemyCharacter.cpp: Triggered Hit");
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		// Sets player velocity to 0 and launches player up
		// for consistent bounce height
		Player->GetCharacterMovement()->Velocity.Z = 0.f;
		Player->LaunchCharacter(FVector(0.f, 0.f, 1000.f), false, false);
		Death();
	}
}

void AEnemyCharacter::BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EnemyCharacter.cpp: Triggered Hit");
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		// Kills player
		Player->Death();
	}
}