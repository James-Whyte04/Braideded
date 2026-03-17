// Fill out your copyright notice in the Description page of Project Settings.


#include "A_TimeDilationObject.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "AC_TimeDilation.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

// Sets default values
AA_TimeDilationObject::AA_TimeDilationObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PrimaryActorTick.bCanEverTick = true;
	RadiusCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RadiusCollider"));
	RadiusCollider->SetupAttachment(RootComponent);
	RadiusCollider->SetSphereRadius(TimeDilationRadius);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(RadiusCollider);
	FlipbookComponent->SetFlipbook(Flipbook);
}

// Called when the game starts or when spawned
void AA_TimeDilationObject::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("hello")));
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

	RadiusCollider->SetSphereRadius(TimeDilationRadius);
	AA_TimeDilationObject::Execute_SetActive(this, false);
}

// Called every frame
void AA_TimeDilationObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



/// <summary>
/// OBJECT POOLING FUNCTIONS
/// </summary>

void AA_TimeDilationObject::SetActive_Implementation(bool Active)
{
	isActive = Active;
	SetActorHiddenInGame(!Active);
}

bool AA_TimeDilationObject::IsActive_Implementation()
{
	return isActive;
}



/// <summary>
/// REWIND INTERFACE FUNCTIONS
/// </summary>

FCharacterData AA_TimeDilationObject::IGetCharacterSnapshot_Implementation()
{
	float PlaybackTime = FlipbookComponent->GetPlaybackPosition();

	FCharacterData object = FCharacterData(GetActorLocation(),
		GetActorRotation(),
		FVector(0.f, 0.f, 0.f),
		FlipbookComponent->GetFlipbook(),
		FlipbookComponent->GetFlipbook()->GetKeyFrameIndexAtTime(PlaybackTime),
		MOVE_None,
		isActive);

	return object;
}

void AA_TimeDilationObject::ISetCharacterSnapshot_Implementation(FCharacterData CharData)
{
	SetActorLocation(CharData.CharacterPosition);
	SetActorRotation(CharData.CharacterRotation);
	FlipbookComponent->SetFlipbook(CharData.Flipbook);
	FlipbookComponent->SetPlaybackPosition(CharData.FlipbookFrame, true);
	AA_TimeDilationObject::Execute_SetActive(this, CharData.IsVisible);
}

void AA_TimeDilationObject::IEnterRewindState_Implementation()
{

}

void AA_TimeDilationObject::IExitRewindState_Implementation(FCharacterData CharData)
{
	if (!isActive) return;
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerController::StaticClass());
	
	if (!actor) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(actor))
	{
		UInputMappingContext* IMC = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/InputMappingContext/IMC_TimeDilation.IMC_TimeDilation"));
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(IMC, 0);

			PlayerController = nullptr;
			delete PlayerController;

			Subsystem = nullptr;
			delete Subsystem;

			IMC = nullptr;
			delete IMC;
		}
	}
	actor = nullptr;
	delete actor;
}