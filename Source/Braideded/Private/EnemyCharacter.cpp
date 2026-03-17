// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.h"
#include "PaperTileMapActor.h"
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
	isVisible = true;
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

	HeadCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::Hit);
	FloorChecker->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CheckFloor);
	WallChecker->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CheckWall);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::Death()
{
	isDead = true;
	FlipbookComponent->SetFlipbook(DeathFlipbook);
	DisableCollision();
	LaunchCharacter(FVector(0.f, 0.f, 100.f), false, false);

	DELAY(2.f,
		{ Execute_Despawn(this); });
}

void AEnemyCharacter::Move(float DeltaTime)
{
	if (!isVisible || isDead)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NotWalking")));
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
}

void AEnemyCharacter::DisableCollision()
{
	Super::DisableCollision();
	
	HeadCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FloorChecker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}




void AEnemyCharacter::ChangeDirection()
{
	FRotator Rotation = GetActorRotation();
	FTransform NewTransform = FTransform(FRotator(Rotation.Pitch, Rotation.Yaw - 180.f, Rotation.Roll),
		GetActorLocation(),
		GetActorScale());

	SetActorTransform(NewTransform);
}

void AEnemyCharacter::CheckWall(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Triggered Wall Check")));
	APaperTileMapActor* Map = Cast<APaperTileMapActor>(OtherActor);
	if (Map)
	{
		ChangeDirection();
	}
	else return;

	//do same check for projectile when implemented
}

void AEnemyCharacter::CheckFloor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Triggered Floor Check")));
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr && !canFall)
	{
		ChangeDirection();
	}
}

void AEnemyCharacter::Hit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Triggered Hit")));
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		Player->LaunchCharacter(FVector(0.f, 0.f, 1750.f), false, false);
		Death();
	}
}