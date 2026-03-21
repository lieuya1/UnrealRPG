#include "AnimInstance/TPAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UTPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* PawnOwner = TryGetPawnOwner();
	if (!PawnOwner) return;

	BaseCharacter = Cast<ABaseCharacter>(PawnOwner);
	if (!BaseCharacter) return;

	CharacterMovementComponent = BaseCharacter->GetCharacterMovement();

}

void UTPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!BaseCharacter || !CharacterMovementComponent) return;

	// Movement Updates
	bIsAccelerating = CharacterMovementComponent->GetCurrentAcceleration().Size() > 0.f;
	GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);

	// Rotation Updates
	// Set Aim rotation base on character's current aim direction
	AimRotation = BaseCharacter->GetBaseAimRotation();
	// Calculate rotation direction base on current velocity
	MovementRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());
	// Calculate movement offset yaw to determine the difference between aim and movement direction
	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	// Orientation update
	BaseCharacter->bUseControllerRotationYaw = bIsAccelerating;
}
