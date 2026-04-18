#include "Component/TPAbilitySystemComponent.h"

#include "Ability/TPComboAttackAbility.h"
#include "Ability/TPGameplayAbility.h"

void UTPAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UTPGameplayAbility>>& Abilities)
{
	for (auto AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UTPGameplayAbility* TPAbility = Cast<UTPGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AppendTags(TPAbility->InputTags);
			GiveAbility(AbilitySpec);
		}
	}
}

void UTPAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTag(InputTag))
		{
			AbilitySpecInputPressed(Spec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
				Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());

			// ✅ If ability is active and is a combo ability notify it of input
			if (Spec.IsActive())
			{
				if (UTPComboAttackAbility* ComboAbility = 
					Cast<UTPComboAttackAbility>(Spec.GetPrimaryInstance()))
				{
					ComboAbility->OnComboInputPressed();
					return; // ← don't activate new ability, just buffer input
				}
				return;
			}

			TryActivateAbility(Spec.Handle);
		}
	}
}

void UTPAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	for (FGameplayAbilitySpec& Spec :GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTag(InputTag))
		{
			AbilitySpecInputReleased(Spec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
				Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}