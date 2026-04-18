// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TPExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UTPExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	UTPExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
