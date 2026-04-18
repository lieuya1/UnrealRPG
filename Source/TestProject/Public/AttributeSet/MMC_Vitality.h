// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Vitality.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UMMC_Vitality : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_Vitality();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
