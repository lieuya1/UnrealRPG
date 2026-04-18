// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/MMC_Intelligence.h"
#include "Character/BaseCharacter.h"


UMMC_Intelligence::UMMC_Intelligence()
{
}

float UMMC_Intelligence::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FGameplayEffectContextHandle GameplayEffectContextHandle = Spec.GetContext();
	const ABaseCharacter* SourceCharacter = Cast<ABaseCharacter>(GameplayEffectContextHandle.GetSourceObject());

	float Intelligence = 10.f;

	if (!SourceCharacter) return Intelligence;
	return Intelligence * SourceCharacter->CharacterLevel;
}
