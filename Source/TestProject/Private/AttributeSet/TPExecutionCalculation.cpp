// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/TPExecutionCalculation.h"
#include "AttributeSet/TPAttributeSet.h"
#include "Tags/TPGameplayTags.h"

struct FTPDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);

	FTPDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPAttributeSet, PhysicalAttack, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPAttributeSet, PhysicalResistance, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPAttributeSet, CriticalChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPAttributeSet, CriticalDamage, Source, true);
	}
};

static const FTPDamageStatics& DamageStatics()
{
	static FTPDamageStatics Statics;
	return Statics;
}

UTPExecutionCalculation::UTPExecutionCalculation()
{
	const auto& Statics = DamageStatics();
	RelevantAttributesToCapture.Add(Statics.PhysicalAttackDef);
	RelevantAttributesToCapture.Add(Statics.PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(Statics.CriticalChanceDef);
	RelevantAttributesToCapture.Add(Statics.CriticalDamageDef);
}

void UTPExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto& Statics = DamageStatics();

	FAggregatorEvaluateParameters Params;
	Params.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	Params.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float SourcePhysicalAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.PhysicalAttackDef, Params, SourcePhysicalAttack);
	SourcePhysicalAttack = FMath::Max<float>(SourcePhysicalAttack, 0.f);

	float TargetPhysicalResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.PhysicalResistanceDef, Params, TargetPhysicalResistance);
	TargetPhysicalResistance = FMath::Max<float>(TargetPhysicalResistance, 0.f);

	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.CriticalChanceDef, Params, SourceCriticalChance);
	SourceCriticalChance = FMath::Clamp<float>(SourceCriticalChance, 0.f, 1.f);

	float SourceCriticalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.CriticalDamageDef, Params, SourceCriticalDamage);
	SourceCriticalDamage = FMath::Clamp<float>(SourceCriticalDamage, 0.f, 1.f);

	float AbilityDamage = Spec.GetSetByCallerMagnitude(FTPGameplayTags::Get().Damage);
	float EffectiveDamage = FMath::Max(0.f, (SourcePhysicalAttack + AbilityDamage) - TargetPhysicalResistance);

	if (const bool bIsCritical = FMath::RandRange(0.f, 1.f) <= SourceCriticalChance)
	{
		EffectiveDamage *= 1.f + SourceCriticalDamage;

		FGameplayEffectSpec* MutableSpec = const_cast<FGameplayEffectSpec*>(&ExecutionParams.GetOwningSpec());
		if (MutableSpec)
		{
			MutableSpec->DynamicGrantedTags.AddTag(FTPGameplayTags::Get().Hit_Critical);
		}
	}

	float Damage = EffectiveDamage;
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UTPAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage));
}
