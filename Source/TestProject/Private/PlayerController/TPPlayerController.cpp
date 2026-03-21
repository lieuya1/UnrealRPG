#include "PlayerController/TPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ATPPlayerController:: ATPPlayerController()
{
	bReplicates = true;
}

void ATPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//check and add input mapping context

	if (InputContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
	}
}

void ATPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Cast input component to enhanced input component
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPPlayerController::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPPlayerController::Look);
	}
}

void ATPPlayerController::Move(const FInputActionValue& MoveActionValue)
{
	//Retrieve 2D input vector (x: right/left, y: forward/backward)
	const FVector2d InputVector = MoveActionValue.Get<FVector2D>();

	//Get Yaw rotation from controller from movement direction
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	//Calculate forward and right directions based on yaw rotation
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//Add movement input to pawn if valid
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(Forward, InputVector.Y);
		ControlledPawn->AddMovementInput(Right, InputVector.X);
	}
}

void ATPPlayerController::Look(const FInputActionValue& LookActionValue)
{
	//Get look input as 2d vector
	const FVector2D LookVector = LookActionValue.Get<FVector2D>();

	//Apply yaw (horizontal) and pitch (vertical) rotation input
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

