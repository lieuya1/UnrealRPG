#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AnimInstance/TPAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerState/TPPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Component/CombatComponent.h"
#include "PlayerController/TPPlayerController.h"
#include "UI/TPHUD.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bUsePawnControlRotation = true; // move spring arm with mouse

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false; // prevents camera from moving independently

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server
	InitializeAbilitySystem();

	AddAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client
	InitializeAbilitySystem();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::InitializeAbilitySystem()
{
	ATPPlayerState* TPPlayerState = GetPlayerState<ATPPlayerState>();
	check(TPPlayerState);
	TPPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	AbilitySystemComponent = TPPlayerState->GetAbilitySystemComponent();
	AttributeSet = TPPlayerState->GetAttributeSet();
	if (GetCombatComponent()) { GetCombatComponent()->BindTagChangedCallbacks(); }
	if (GetCombatComponent())
	{
		if (USkeletalMeshComponent* CharacterMesh = GetMesh())
		{
			if (UTPAnimInstance* AnimInstance = Cast<UTPAnimInstance>(CharacterMesh->GetAnimInstance()))
			{
				AnimInstance->SetCombatComponent(GetCombatComponent());
				AnimInstance->BindTagChanged();
			}
		}
	}
	if (HasAuthority()) { InitializeDefaultAttributes(); }

	ATPPlayerController* PlayerController = Cast<ATPPlayerController>(GetController());
	if (PlayerController)
	{
		ATPHUD* HUD = Cast<ATPHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->InitHud();
		}
	}
}
