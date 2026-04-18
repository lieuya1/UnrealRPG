#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPGameplayAbility.generated.h"

UCLASS()
class TESTPROJECT_API UTPGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UTPGameplayAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	FGameplayTagContainer InputTags;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool bShouldUseControllerRotationYaw;

	UFUNCTION(BlueprintCallable)
	void UseControllerRotationYaw(bool bEnable);
};
