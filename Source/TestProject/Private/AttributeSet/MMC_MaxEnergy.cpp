// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/MMC_MaxEnergy.h"
#include "AttributeSet/TPAttributeSet.h"


UMMC_MaxEnergy::UMMC_MaxEnergy()
{
	IntelligenceDef.AttributeToCapture = UTPAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	IntelligenceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxEnergy::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);

	return Intelligence * 5.f;
}
