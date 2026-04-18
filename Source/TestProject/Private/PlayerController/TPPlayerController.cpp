#include "PlayerController/TPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Component/TPAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UI/TPHUD.h"

ATPPlayerController::ATPPlayerController()
{
	bReplicates = true;
}

void ATPPlayerController::Client_ShowDamageNumber_Implementation(float DamageAmount, FVector_NetQuantize WorldLocation,
	bool bIsCritical, bool bIsTaken)
{
	if (ATPHUD* HUD = Cast<ATPHUD>(GetHUD()))
	{
		HUD->ShowDamageNumber(DamageAmount, WorldLocation, bIsCritical, bIsTaken);
	}
}

void ATPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Check and add input mapping context
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
		BindTaggedInputActions(InputTagsMap, this, &ATPPlayerController::AbilityAction);
	}
}

void ATPPlayerController::Move(const FInputActionValue& MoveActionValue)
{
	// Retrieve 2D input vector (X: right/left, Y: forward/backward)
	const FVector2d InputVector = MoveActionValue.Get<FVector2D>();

	// Get Yaw rotation from controller from movement direction
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	// Calculate forward and right directions based on Yaw rotation
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Add movement input to pawn if valid
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(Forward, InputVector.Y);
		ControlledPawn->AddMovementInput(Right, InputVector.X);
	}
}

void ATPPlayerController::Look(const FInputActionValue& LookActionValue)
{
	// Get look input as 2D vector
	const FVector2D LookVector = LookActionValue.Get<FVector2D>();

	// Apply yaw (horizontal) and pitch (vertical) rotation input
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void ATPPlayerController::AbilityAction(FGameplayTag ActionTag, bool bPressed)
{
	if (ASC == nullptr)
	{
		ASC = Cast<UTPAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	if (ASC && bPressed)
	{
		ASC->AbilityInputPressed(ActionTag);
	}
	if (ASC && !bPressed)
	{
		ASC->AbilityInputReleased(ActionTag);
	}
}
