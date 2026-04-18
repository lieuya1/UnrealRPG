#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TPAbilitySystemComponent.generated.h"

class UTPGameplayAbility;

UCLASS()
class TESTPROJECT_API UTPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AddAbilities(const TArray<TSubclassOf<UTPGameplayAbility>>& Abilities);

	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
};
