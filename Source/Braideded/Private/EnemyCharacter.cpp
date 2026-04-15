// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.h"
#include "PaperTileMapActor.h"
#include "Projectile.h"
#include "Spawner.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	//isVisible = true;
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

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyCharacter::BodyHit);
	HeadCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::HeadHit);
	FloorChecker->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CheckFloor);
	WallChecker->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CheckWall);
}




/// <summary>
/// BASE DEATH FUNCTIONALITY
/// </summary>

void AEnemyCharacter::Death()
{
	isDead = true;
	FlipbookComponent->SetFlipbook(DeathFlipbook);
	DisableCollision();
	GetCharacterMovement()->Velocity.Z = 0.f;
	LaunchCharacter(FVector(0.f, 0.f, 100.f), false, false);

	GetWorldTimerManager().SetTimer(DeathHandle, this, &AEnemyCharacter::HandleDespawn, 1.f, false);
}

void AEnemyCharacter::HandleDespawn()
{
	IPoolableObject::Execute_IDespawn(this);
}



/// <summary>
/// BASE MOVE FUNCTIONALITY
/// </summary>

void AEnemyCharacter::Move(float DeltaTime)
{
	if (!isVisible || isDead)
	{
		return;
	}

	//Add velocity
	AddMovementInput(GetActorForwardVector(), WalkSpeed * DeltaTime);

	//Set flipbook
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

	FRotator Rotation = GetActorRotation();
	FTransform NewTransform = FTransform(FRotator(Rotation.Pitch, Rotation.Yaw - 180.f, Rotation.Roll),
		GetActorLocation(),
		GetActorScale());

	SetActorTransform(NewTransform);

	GetWorldTimerManager().SetTimerForNextTick([this]()
		{ canChangeDirection = true;} );
}




/// <summary>
/// BASE COLLISION FUNCTIONALITY
/// </summary>

void AEnemyCharacter::EnableCollision()
{
	Super::EnableCollision();

	HeadCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (canFall)
	{
		FloorChecker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
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
		ChangeDirection();
		return;
	}

	else if (Cast<ASpawner>(OtherActor))
	{
		ChangeDirection();
		return;
	}

	else if (Cast<AEnemyCharacter>(OtherActor) && OtherActor != this)
	{
		ChangeDirection();
		return;
	}

	//do same check for projectile when implemented
}

void AEnemyCharacter::CheckFloor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EnemyCharacter.cpp: Triggered Floor Check");
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr && !canFall)
	{
		ChangeDirection();
	}
}

void AEnemyCharacter::HeadHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EnemyCharacter.cpp: Triggered Hit");
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
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
		Player->Death();
	}
}