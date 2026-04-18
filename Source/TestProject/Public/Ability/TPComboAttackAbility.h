#pragma once

#include "CoreMinimal.h"
#include "Ability/TPGameplayAbility.h"
#include "TPComboAttackAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;

UCLASS()
class TESTPROJECT_API UTPComboAttackAbility : public UTPGameplayAbility
{
	GENERATED_BODY()

public:
	UTPComboAttackAbility();
	void OnComboWindowClosed();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	void OnComboWindowOpened();
	void OnComboInputPressed();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	UAnimMontage* ComboMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	int32 MaxComboCount = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	float ComboWindowDuration = 0.5f;

private:
	void CheckComboAndContinue();
	void EndComboEarly();
	
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;

	int32 CurrentComboIndex = 0;
	bool bComboWindowOpen = false;
	bool bInputBuffered = false;
    
	FTimerHandle ComboWindowTimerHandle;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnMontageInterrupted();
};