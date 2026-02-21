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
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FlipbookComponent = GetSprite();
	CharacterComponent = GetCharacterMovement();
}





// DEFAULT UE FUNCTIONS

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SetActions();

	isGrounded = true;
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Walk);

		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);

		EIC->BindAction(IAction1, ETriggerEvent::Triggered, this, &APlayerCharacter::ActivateAction1);
		EIC->BindAction(IAction2, ETriggerEvent::Triggered, this, &APlayerCharacter::ActivateAction2);
		EIC->BindAction(IAction3, ETriggerEvent::Triggered, this, &APlayerCharacter::ActivateAction3);

		EIC->BindAction(IDeactivateAction1, ETriggerEvent::Triggered, this, &APlayerCharacter::FDeactivateAction1);
		EIC->BindAction(IDeactivateAction2, ETriggerEvent::Triggered, this, &APlayerCharacter::FDeactivateAction2);
		EIC->BindAction(IDeactivateAction3, ETriggerEvent::Triggered, this, &APlayerCharacter::FDeactivateAction3);
	}
}





// MOVEMENT FUNCTIONS

void APlayerCharacter::StopWalk(const FInputActionValue& Value) 
{
	if (!CharacterComponent->IsMovingOnGround()) return;
	FlipbookComponent->SetFlipbook(IdleFlipbook);
}

void APlayerCharacter::Walk(const FInputActionValue& Value)
{
	if (CharacterComponent->IsFalling()) 
	{
		FlipbookComponent->SetFlipbook(JumpFlipbook);
	}

	if (Value.Get<float>() > 0) 
	{
		AddMovementInput(FVector::ForwardVector, Value.Get<float>());
		FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

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
	FlipbookComponent->SetFlipbook(IdleFlipbook);
}





//ABILITY ACTIVATE FUNCTIONS
void APlayerCharacter::ActivateAction1(const FInputActionValue& Value)
{
	// Implement action 1 behavior here
	if (ActionObj1 == nullptr) return;
	

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(DefaultMappingContext);
			Subsystem->AddMappingContext(Ability1MappingContext, 0);
		}
	}

	IAction::Execute_IActivate(ActionObj1, Value.Get<float>());
}

void APlayerCharacter::ActivateAction2(const FInputActionValue& Value)
{
	// Implement action 2 behavior here
}

void APlayerCharacter::ActivateAction3(const FInputActionValue& Value)
{
	// Implement action 3 behavior here
}


//ABILITY DEACTIVATE FUNCTIONS
void APlayerCharacter::FDeactivateAction1(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Deactivating");
	if (ActionObj1 == nullptr) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(Ability1MappingContext);
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	IAction::Execute_IDeactivate(ActionObj1);
}

void APlayerCharacter::FDeactivateAction2(const FInputActionValue& Value)
{
	// Implement action 2 deactivation behavior here
}

void APlayerCharacter::FDeactivateAction3(const FInputActionValue& Value)
{
	// Implement action 3 deactivation behavior here
}



// REWIND INTERFACE FUNCTIONS
FCharacterData APlayerCharacter::IGetCharacterSnapshot_Implementation() 
{
	float PlaybackTime = FlipbookComponent->GetPlaybackPosition();

	FCharacterData Char = FCharacterData(GetActorLocation(),
		FlipbookComponent->GetRelativeRotation(),
		GetVelocity(),
		FlipbookComponent->GetFlipbook(),
		FlipbookComponent->GetFlipbook()->GetKeyFrameIndexAtTime(PlaybackTime),
		GetCharacterMovement()->MovementMode);
	return Char;
}

void APlayerCharacter::ISetCharacterSnapshot_Implementation(FCharacterData CharData)
{
	// Set character pos, rot, flipbook, flipbook state
	SetActorLocation(CharData.CharacterPosition);
	FlipbookComponent->SetRelativeRotation(CharData.CharacterRotation);
	FlipbookComponent->SetFlipbook(CharData.Flipbook);
	FlipbookComponent->SetPlaybackPosition(CharData.FlipbookFrame, true);
}

void APlayerCharacter::IEnterRewindState_Implementation() 
{
	GetCharacterMovement()->DisableMovement();
	FlipbookComponent->Stop();
}

void APlayerCharacter::IExitRewindState_Implementation(FCharacterData CharData)
{
	//Set movement mode
	GetCharacterMovement()->SetMovementMode(CharData.MovementMode);

	//Set position and rotation
	SetActorLocation(CharData.CharacterPosition);
	FlipbookComponent->SetRelativeRotation(CharData.CharacterRotation);

	//Set velocity
	GetCharacterMovement()->Velocity = CharData.Velocity;

	//Set flipbook and frame
	FlipbookComponent->Play();
	GetSprite()->SetFlipbook(CharData.Flipbook);
	GetSprite()->SetPlaybackPositionInFrames(CharData.FlipbookFrame, false);
}

void APlayerCharacter::IRewind_Implementation() 
{
	// Set world time to -1
	// Restore positions of rewindable objects
}

void APlayerCharacter::IRollback_Implementation() 
{
	// Set world time to 1
	// Set objects in motion again
}

void APlayerCharacter::SetActions()
{
	if (Action1)
	{
		ActionComponent1 = NewObject<UActorComponent>(this, Action1);
		ActionComponent1->RegisterComponent();

		ActionObj1 = Cast<UObject>(ActionComponent1);
	}
}

void APlayerCharacter::ClearActions()
{
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

	if (ActionComponent3)
	{
		ActionComponent3->DestroyComponent();
		ActionComponent3 = nullptr;
		ActionObj3 = nullptr;
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearActions();
}
