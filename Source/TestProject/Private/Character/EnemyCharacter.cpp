#include "Character/EnemyCharacter.h"
#include "AnimInstance/TPAnimInstance.h"
#include "Component/TPAbilitySystemComponent.h"
#include "Component/CombatComponent.h"
#include "TestProject/Public/AttributeSet/TPAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTPAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTPAttributeSet>("AttributeSet");
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
	AddAbilities();
}

void AEnemyCharacter::InitializeAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
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
}


