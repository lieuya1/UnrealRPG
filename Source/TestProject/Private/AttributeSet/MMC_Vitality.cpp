// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/MMC_Vitality.h"
#include "Character/BaseCharacter.h"


UMMC_Vitality::UMMC_Vitality()
{
}

float UMMC_Vitality::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FGameplayEffectContextHandle GameplayEffectContextHandle = Spec.GetContext();
	const ABaseCharacter* SourceCharacter = Cast<ABaseCharacter>(GameplayEffectContextHandle.GetSourceObject());

	float Vitality = 10.f;

	if (!SourceCharacter) return Vitality;
	return Vitality * SourceCharacter->CharacterLevel;
}