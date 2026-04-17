// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AC_Rewind.h"

// Description: Player character class, inherits from AMyCharacter,
// contains player mechanics such as movement and actions


// Constructor
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	WalkSpeed = 300.f;
	SprintSpeed = 600.f;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 500.f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	FlipbookComponent = GetSprite();
	CharacterComponent = GetCharacterMovement();

	CharacterComponent->MaxWalkSpeed = WalkSpeed;
	CharacterComponent->bOrientRotationToMovement = false;
}




void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}

	SetActions();
	IPoolableObject::Execute_ISetActive(this, true);

	isGrounded = true;
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearActions();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EIC->BindAction(IA_MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Walk);

		EIC->BindAction(IA_JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
		EIC->BindAction(IA_JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);

		EIC->BindAction(IA_Action1, ETriggerEvent::Triggered, this, &APlayerCharacter::ActivateAction1);
		EIC->BindAction(IA_Action2, ETriggerEvent::Triggered, this, &APlayerCharacter::ActivateAction2);

		EIC->BindAction(IA_DeactivateAction1, ETriggerEvent::Triggered, this, &APlayerCharacter::DeactivateAction1);
		EIC->BindAction(IA_DeactivateAction2, ETriggerEvent::Triggered, this, &APlayerCharacter::DeactivateAction2);
	}
}




/// Movement functions

void APlayerCharacter::StopWalk(const FInputActionValue& Value) 
{
	if (!CharacterComponent->IsMovingOnGround()) return;
	FlipbookComponent->SetFlipbook(IdleFlipbook);
}

void APlayerCharacter::Walk(const FInputActionValue& Value)
{
	// Set jump flipbook if player is airborne
	if (CharacterComponent->IsFalling()) 
	{
		FlipbookComponent->SetFlipbook(JumpFlipbook);
	}

	if (Value.Get<float>() > 0) 
	{
		// Adds movement input and sets flipbook rotation based on movement direction
		AddMovementInput(FVector::ForwardVector, Value.Get<float>());
		FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		// Set walk flipbook if player is grounded
		if (!CharacterComponent->IsMovingOnGround()) return;
		FlipbookComponent->SetFlipbook(WalkFlipbook);
	}
	else if (Value.Get<float>() < 0)
	{
		AddMovementInput(FVector::ForwardVector, Value.Get<float>());
		FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

		if (!CharacterComponent->IsMovingOnGround()) return;
		FlipbookComponent->SetFlipbook(WalkFlipbook);
	}
	else 
	{
		// Set idle flipbook if player is grounded and not moving
		if (!CharacterComponent->IsMovingOnGround()) return;
		FlipbookComponent->SetFlipbook(IdleFlipbook);
	}
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	Jump();
	FlipbookComponent->SetFlipbook(JumpFlipbook);
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (CharacterComponent->GetCurrentAcceleration().X != 0) return;

	// Set flipbook to idle if not moving on landing 
	FlipbookComponent->SetFlipbook(IdleFlipbook);
}




//Action functions

// Activate functions
void APlayerCharacter::ActivateAction1(const FInputActionValue& Value)
{
	// Crash prevention: Prevents function from being called on null object
	if (ActionObj1 == nullptr) return;
	

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Removes the default input mapping context,
			// adds the ability input mapping context
			Subsystem->RemoveMappingContext(IMC_Default);
			Subsystem->AddMappingContext(IMC_Ability1, 0);
		}
	}
	// Executes action activate function
	IAction::Execute_IActivate(ActionObj1, Value.Get<float>());
}

void APlayerCharacter::ActivateAction2(const FInputActionValue& Value)
{
	if (ActionObj2 == nullptr) return;


	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(IMC_Default);
			Subsystem->AddMappingContext(IMC_Ability2, 0);
		}
	}

	IAction::Execute_IActivate(ActionObj2, Value.Get<float>());
}



// Deactivate functions
void APlayerCharacter::DeactivateAction1(const FInputActionValue& Value)
{
	// Crash prevention: Prevents function from being called on null object
	if (ActionObj1 == nullptr) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Removes the ability input mapping context,
			// reverts to the default input mapping context
			Subsystem->RemoveMappingContext(IMC_Ability1);
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}
	// Executes action deactivate function
	IAction::Execute_IDeactivate(ActionObj1);
}

void APlayerCharacter::DeactivateAction2(const FInputActionValue& Value)
{
	if (ActionObj2 == nullptr) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(IMC_Ability2);
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}

	IAction::Execute_IDeactivate(ActionObj2);
}




// Rewind Interface Functions

FCharacterData APlayerCharacter::IGetCharacterSnapshot_Implementation() 
{
	float PlaybackTime = FlipbookComponent->GetPlaybackPosition();

	FCharacterData Char = FCharacterData(GetActorLocation(),
		FlipbookComponent->GetRelativeRotation(),
		GetVelocity(),
		FlipbookComponent->GetFlipbook(),
		FlipbookComponent->GetFlipbook()->GetKeyFrameIndexAtTime(PlaybackTime),
		CharacterComponent->MovementMode,
		isVisible,
		isDead);
	return Char;
}

void APlayerCharacter::ISetCharacterSnapshot_Implementation(FCharacterData CharData)
{
	// Set character pos, rot, flipbook, flipbook state
	SetActorLocation(CharData.CharacterPosition);
	FlipbookComponent->SetRelativeRotation(CharData.CharacterRotation);
	FlipbookComponent->SetFlipbook(CharData.Flipbook);
	FlipbookComponent->SetPlaybackPosition(CharData.Frame, true);
	isVisible = CharData.IsVisible;
	isDead = CharData.IsDead;
}

void APlayerCharacter::IEnterRewindState_Implementation() 
{
	CharacterComponent->DisableMovement();
	FlipbookComponent->Stop();
}

void APlayerCharacter::IExitRewindState_Implementation(FCharacterData CharData)
{
	EnableCollision();
	//Set movement mode
	CharacterComponent->SetMovementMode(CharData.MovementMode);

	//Set position and rotation
	SetActorLocation(CharData.CharacterPosition);
	FlipbookComponent->SetRelativeRotation(CharData.CharacterRotation);

	//Set velocity
	CharacterComponent->Velocity = CharData.Velocity;

	FlipbookComponent->Play();

	// Sets the correct flipbook based on movement mode and acceleration
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
}




// Time Dilation Interface Functions

void APlayerCharacter::IApplyDilationFactor_Implementation(float Factor) 
{
	this->CustomTimeDilation = Factor;
}

void APlayerCharacter::IClearTimeDilation_Implementation()
{
	this->CustomTimeDilation = 1.f;
}




/// Action Functions

int APlayerCharacter::RewindCheck()
{
	// Checks if either ActionComponent is a UAC_Rewind,
	// returns 1 if ActionComponent1 is UAC_Rewind,
	// 2 if ActionComponent2 is UAC_Rewind,
	// and 0 if neither are UAC_Rewind
	UAC_Rewind* RewindComponent = Cast<UAC_Rewind>(ActionComponent1);

	if (RewindComponent)
	{
		return 1;
	}

	RewindComponent = Cast<UAC_Rewind>(ActionComponent2);

	if (RewindComponent)
	{
		return 2;
	}

	return 0;
}

void APlayerCharacter::SetActions()
{
	// Spawns the Action Actor Components based on the
	// Action1 and Action2 TSubclass variables set in editor
	if (Action1)
	{
		ActionComponent1 = NewObject<UActorComponent>(this, Action1);
		ActionComponent1->RegisterComponent();

		ActionObj1 = Cast<UObject>(ActionComponent1);
	}
	if (Action2)
	{
		ActionComponent2 = NewObject<UActorComponent>(this, Action2);
		ActionComponent2->RegisterComponent();

		ActionObj2 = Cast<UObject>(ActionComponent2);
	}
}

void APlayerCharacter::ClearActions()
{
	// Clears the Action Actor Components from the player character
	if (ActionComponent1)
	{
		ActionComponent1->DestroyComponent();
		ActionComponent1 = nullptr;
		ActionObj1 = nullptr;
	}
	if (ActionComponent2)
	{
		ActionComponent2->DestroyComponent();
		ActionComponent2 = nullptr;
		ActionObj2 = nullptr;
	}
}



void APlayerCharacter::Death()
{
	AMyCharacter::Death();
	DisableCollision();
	GetMovementComponent()->Velocity.Z = 0;
	LaunchCharacter(FVector(0.f, 0.f, 100.f), false, false);
	
	int RewindComponent = RewindCheck();

	if (RewindComponent == 1)
	{
		// Activate action 1 if Action1 is UAC_Rewind
		DELAY(1.f,
			{ ActivateAction1(1); });
	}
	else if (RewindComponent == 2)
	{
		// Activate action 2 if Action2 is UAC_Rewind
		DELAY(1.f,
			{ ActivateAction2(1); });
	}
	else
	{
		// Restart level if player does not have UAC_Rewind Action
		DELAY(1.f,
			{ UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false); });
	}
}
