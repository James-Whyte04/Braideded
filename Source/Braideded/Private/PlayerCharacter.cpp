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
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
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

	AbilityComponent1 = CreateDefaultSubobject<UAC_Rewind>(TEXT("AbilityComponent1"));
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

	if (AbilityComponent1 && AbilityComponent1->GetClass()->ImplementsInterface(UAction::StaticClass()))
	{
		Ability1 = Cast<UObject>(AbilityComponent1);

		if (Ability1 != nullptr) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Casted"));
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("NO CAST"));
		}
	}

	isGrounded = true;
	isRewinding = false;
	canRecord = true;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	// check if ability component is an IAction 

	if (Ability1 == nullptr || !canRecord) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Recording");
	IAction::Execute_IRecord(Ability1);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::IMove);

		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::IJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::IStopJumping);

		EIC->BindAction(Action1, ETriggerEvent::Triggered, this, &APlayerCharacter::IAction1);
		EIC->BindAction(Action2, ETriggerEvent::Triggered, this, &APlayerCharacter::IAction2);
		EIC->BindAction(Action3, ETriggerEvent::Triggered, this, &APlayerCharacter::IAction3);

		EIC->BindAction(DeactivateAction1, ETriggerEvent::Triggered, this, &APlayerCharacter::IDeactivateAction1);
	}
}





// MOVEMENT FUNCTIONS

void APlayerCharacter::IStopMove(const FInputActionValue& Value) 
{
	if (!CharacterComponent->IsMovingOnGround()) return;
	FlipbookComponent->SetFlipbook(IdleFlipbook);
}

void APlayerCharacter::IMove(const FInputActionValue& Value)
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

void APlayerCharacter::IJump(const FInputActionValue& Value)
{
	Jump();
	FlipbookComponent->SetFlipbook(JumpFlipbook);
}

void APlayerCharacter::IStopJumping(const FInputActionValue& Value)
{
	StopJumping();
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (CharacterComponent->GetCurrentAcceleration().X != 0) return;
	FlipbookComponent->SetFlipbook(IdleFlipbook);
}





// ABILITY FUNCTIONS

void APlayerCharacter::IAction1(const FInputActionValue& Value)
{
	canRecord = false;
	// Implement action 1 behavior here
	if (Ability1 == nullptr) return;
	
	if (!isRewinding) 
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(RewindMappingContext, 0);
				isRewinding = true;
			}
		}
	}

	IAction::Execute_IActivate(Ability1, Value.Get<float>());
}

void APlayerCharacter::IAction2(const FInputActionValue& Value)
{
	// Implement action 2 behavior here
}

void APlayerCharacter::IAction3(const FInputActionValue& Value)
{
	// Implement action 3 behavior here
}

void APlayerCharacter::IDeactivateAction1(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Deactivating");
	if (Ability1 == nullptr) return;
	if (isRewinding) 
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(RewindMappingContext);
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				isRewinding = false;
			}
		}
	}

	IAction::Execute_IDeactivate(Ability1);
	canRecord = true;
}




// REWIND INTERFACE FUNCTIONS

FCharacterData APlayerCharacter::IGetCharacterSnapshot_Implementation() 
{
	float PlaybackTime = FlipbookComponent->GetPlaybackPosition();

	FCharacterData Char = FCharacterData(GetActorLocation(),
		GetActorRotation(),
		GetVelocity(),
		FlipbookComponent->GetFlipbook(),
		FlipbookComponent->GetFlipbook()->GetKeyFrameIndexAtTime(PlaybackTime),
		GetCharacterMovement()->MovementMode);
	return Char;
}

void APlayerCharacter::ISetCharacterSnapshot_Implementation(FCharacterData CharData)
{
	// Set character pos, rot, flipbook, flipbook state
	SetActorLocationAndRotation(CharData.CharacterPosition, CharData.CharacterRotation);
	FlipbookComponent -> SetFlipbook(CharData.Flipbook);
	FlipbookComponent -> SetPlaybackPosition(CharData.FlipbookFrame, true);
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
	SetActorLocationAndRotation(CharData.CharacterPosition, CharData.CharacterRotation);

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

