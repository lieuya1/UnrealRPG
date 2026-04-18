#include "Ability/TPComboAttackAbility.h"
#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h" 

UTPComboAttackAbility::UTPComboAttackAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UTPComboAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    CurrentComboIndex = 0;
    bComboWindowOpen = false;
    bInputBuffered = false;

    if (!ComboMontage)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        FName("PlayComboMontage"),
        ComboMontage,
        1.f,    // PlayRate
        NAME_None, // StartSection
        false   // bStopWhenAbilityEnds
    );

    if (MontageTask)
    {
        // Bind delegates
        MontageTask->OnCompleted.AddDynamic(this, &UTPComboAttackAbility::OnMontageCompleted);
        MontageTask->OnBlendOut.AddDynamic(this, &UTPComboAttackAbility::OnMontageBlendOut);
        MontageTask->OnCancelled.AddDynamic(this, &UTPComboAttackAbility::OnMontageCancelled);
        MontageTask->OnInterrupted.AddDynamic(this, &UTPComboAttackAbility::OnMontageInterrupted);

        MontageTask->ReadyForActivation();
    }
}

void UTPComboAttackAbility::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPComboAttackAbility::OnMontageBlendOut()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPComboAttackAbility::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPComboAttackAbility::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPComboAttackAbility::OnComboWindowOpened()
{
    bComboWindowOpen = true;
    CurrentComboIndex++;
}

void UTPComboAttackAbility::OnComboInputPressed()
{
    if (bComboWindowOpen)
    {
        bInputBuffered = true;

        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(ComboWindowTimerHandle);
        }
        bComboWindowOpen = false;
    }
}

void UTPComboAttackAbility::OnComboWindowClosed()
{
    if (!bInputBuffered)
    {
        EndComboEarly();
    }
    bComboWindowOpen = false;
    bInputBuffered = false;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ComboWindowTimerHandle);
    }
}

void UTPComboAttackAbility::CheckComboAndContinue()
{
    if (CurrentComboIndex >= MaxComboCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Max combo reached!"));
    }
}

void UTPComboAttackAbility::EndComboEarly()
{
    ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if (!Character) return;

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
    if (!AnimInstance) return;

    AnimInstance->Montage_Stop(0.2f, ComboMontage);
}

void UTPComboAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled)
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ComboWindowTimerHandle);
    }

    CurrentComboIndex = 0;
    bComboWindowOpen = false;
    bInputBuffered = false;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}