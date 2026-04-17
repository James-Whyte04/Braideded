// Fill out your copyright notice in the Description page of Project Settings.


#include "A_TimeDilationObject.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "AC_TimeDilation.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

// Description: The actor that is spawned in scene
// when the time dilation action is activated,
// sends the actors that enter its radius to the 
// Time Dilation Component to time dilation to them


// Constructor
AA_TimeDilationObject::AA_TimeDilationObject()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PrimaryActorTick.bCanEverTick = true;

	// Radius Collider setup,
	// Radius size is set in Begin Play with
	// the value set in the Time Dilation Component
	RadiusCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RadiusCollider"));
	RadiusCollider->SetupAttachment(RootComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(RadiusCollider);
	FlipbookComponent->SetFlipbook(Flipbook);
}




void AA_TimeDilationObject::BeginPlay()
{
	Super::BeginPlay();
	AA_TimeDilationObject::Execute_ISetActive(this, false);
}




// Function to set up the parameters of the time dilation object,
// called by the Time Dilation Component on Begin Play

void AA_TimeDilationObject::SetUpParameters(UAC_TimeDilation* TimeDilationAC, float Radius, float DilationFactor)
{
	if (!TimeDilationAC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TimeDilationObject: CAST TO TIME DILATION COMPONENT FAILED")));
		return;
	}

	// Binds overlap events to the functions of the TimeDilation Component
	RadiusCollider->OnComponentBeginOverlap.AddDynamic(TimeDilationAC, &UAC_TimeDilation::AddToAffectedActors);
	RadiusCollider->OnComponentEndOverlap.AddDynamic(TimeDilationAC, &UAC_TimeDilation::RemoveFromAffectedActors);

	// Set the radius using the radius in the Time Dilation Component
	RadiusCollider->SetSphereRadius(Radius);
	AA_TimeDilationObject::Execute_ISetActive(this, false);
}




/// OBJECT POOLING FUNCTIONS

void AA_TimeDilationObject::SetActive_Implementation(bool Active)
{
	isActive = Active;
	SetActorHiddenInGame(!Active);
}

bool AA_TimeDilationObject::IsActive_Implementation()
{
	return isActive;
}



/// REWIND INTERFACE FUNCTIONS

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
	FlipbookComponent->SetPlaybackPosition(CharData.Frame, true);
	AA_TimeDilationObject::Execute_ISetActive(this, CharData.IsVisible);
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