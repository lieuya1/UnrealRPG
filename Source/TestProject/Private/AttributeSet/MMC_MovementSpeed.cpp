// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/MMC_MovementSpeed.h"
#include "AttributeSet/TPAttributeSet.h"


UMMC_MovementSpeed::UMMC_MovementSpeed()
{
	MovementSpeedDef.AttributeToCapture = UTPAttributeSet::GetMovementSpeedAttribute();
	MovementSpeedDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MovementSpeedDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(MovementSpeedDef);
}

float UMMC_MovementSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MovementSpeed = 0.f;
	GetCapturedAttributeMagnitude(MovementSpeedDef, Spec, EvaluationParameters, MovementSpeed);

	MovementSpeed = 600.f;

	return MovementSpeed;
}
