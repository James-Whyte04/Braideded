// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "MyCharacter.h"
#include "A_TimeDilationObject.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	SetRootComponent(Collider);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook");
	FlipbookComponent->SetupAttachment(RootComponent);
	FlipbookComponent->SetFlipbook(ProjectileFlipbook);

	Velocity = 100.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	isVisible = true;
	isActive = true;

	Collider->SetGenerateOverlapEvents(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileCollision);
	SetActorTickEnabled(true);
}

void AProjectile::Tick(float DeltaTime)
{
	AddVelocity(DeltaTime);
}




void AProjectile::AddVelocity(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	FVector NewLocation = CurrentLocation + (GetActorForwardVector() * Velocity) * DeltaTime;

	SetActorLocation(NewLocation);
}





//REWIND INTERFACE FUNCTIONS
FCharacterData AProjectile::IGetCharacterSnapshot_Implementation()
{
	float PlaybackTime = FlipbookComponent->GetPlaybackPosition();

	FCharacterData Snapshot = FCharacterData(GetActorLocation(),
		GetActorRotation(),
		GetVelocity(),
		FlipbookComponent->GetFlipbook(),
		FlipbookComponent->GetFlipbook()->GetKeyFrameIndexAtTime(PlaybackTime),
		EMovementMode::MOVE_None,
		isVisible,
		isActive);

	return Snapshot;
}

void AProjectile::ISetCharacterSnapshot_Implementation(FCharacterData CharData)
{
	SetActorLocation(CharData.CharacterPosition);
	SetActorRotation(CharData.CharacterRotation);
	FlipbookComponent->SetFlipbook(CharData.Flipbook);
	FlipbookComponent->SetPlaybackPositionInFrames(CharData.Frame, false);
	isVisible = CharData.IsVisible;
	SetActorHiddenInGame(!isVisible);
}

void AProjectile::IEnterRewindState_Implementation()
{
	FlipbookComponent->Stop();
	SetActorTickEnabled(false);
}

void AProjectile::IExitRewindState_Implementation(FCharacterData CharData)
{
	SetActorLocation(CharData.CharacterPosition);
	SetActorRotation(CharData.CharacterRotation);

	FlipbookComponent->Play();
	FlipbookComponent->SetFlipbook(CharData.Flipbook);
	FlipbookComponent->SetPlaybackPositionInFrames(CharData.Frame, false);

	isVisible = CharData.IsVisible;
	SetActorHiddenInGame(!isVisible);
	IPoolableObject::Execute_ISetActive(this, CharData.IsDead);

	if (isActive)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}




//TIME DILATION INTERFACE FUNCTIONS
void AProjectile::IApplyDilationFactor_Implementation(float factor)
{
	this->CustomTimeDilation = factor;
}

void AProjectile::IClearTimeDilation_Implementation()
{
	this->CustomTimeDilation = 1.f;
}




//POOLABLE OBJECT INTERFACE FUNCTIONS
void AProjectile::ISetActive_Implementation(bool Active)
{
	//set timer to add movement if is active = true
	//remove timer if active = false and timer is active
	isActive = Active;

	if (isActive)
	{
		Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AProjectile::IIsActive_Implementation()
{
	return isActive;
}

void AProjectile::ISpawn_Implementation(FVector SpawnPoint, FRotator SpawnRotation)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AProjectile.cpp: Spawned");
	isVisible = true;
	IPoolableObject::Execute_ISetActive(this, true);
	SetActorHiddenInGame(!isVisible);
	SetActorLocation(SpawnPoint);
	SetActorRotation(SpawnRotation);
	SetActorTickEnabled(true);
}

void AProjectile::IDespawn_Implementation()
{
	isVisible = false;
	IPoolableObject::Execute_ISetActive(this, false);
	SetActorHiddenInGame(!isVisible);
	SetActorLocation(FVector(0.f, 0.f, 0.f));
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
	SetActorTickEnabled(false);
}






void AProjectile::OnProjectileCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character && AMyCharacter::Execute_IIsActive(Character))
	{
		Character->Death();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Killed Character");
		IPoolableObject::Execute_IDespawn(this);
	}
	

	//LAST CHECK: despawns if colliding with object
	if (!Cast<AA_TimeDilationObject>(OtherActor))
	{
		IPoolableObject::Execute_IDespawn(this);
	}
}