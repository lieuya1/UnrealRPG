#include "PlayerState/TPPlayerState.h"
#include "Component/TPAbilitySystemComponent.h"
#include "TestProject/Public/AttributeSet/TPAttributeSet.h"

ATPPlayerState::ATPPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UTPAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UTPAttributeSet>("AttributeSet");
}
