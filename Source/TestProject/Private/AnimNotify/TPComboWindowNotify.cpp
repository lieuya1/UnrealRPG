// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/TPComboWindowNotify.h"
#include "Character/BaseCharacter.h"
#include "Component/TPAbilitySystemComponent.h"
#include "Ability/TPComboAttackAbility.h"


void UTPComboWindowNotify::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	UTPAbilitySystemComponent* ASC = Cast<UTPAbilitySystemComponent>(
		Character->GetAbilitySystemComponent());
	if (!ASC) return;
	
	for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (UTPComboAttackAbility* ComboAbility = 
			Cast<UTPComboAttackAbility>(Spec.GetPrimaryInstance()))
		{
			ComboAbility->OnComboWindowOpened();
			break;
		}
	}
}

void UTPComboWindowNotify::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	UTPAbilitySystemComponent* ASC = Cast<UTPAbilitySystemComponent>(
		Character->GetAbilitySystemComponent());
	if (!ASC) return;
	
	for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (UTPComboAttackAbility* ComboAbility = 
			Cast<UTPComboAttackAbility>(Spec.GetPrimaryInstance()))
		{
			ComboAbility->OnComboWindowClosed();
			break;
		}
	}
}