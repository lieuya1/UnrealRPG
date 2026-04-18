// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPDamageUserWidget.generated.h"

class UTextBlock;

UCLASS()
class TESTPROJECT_API UTPDamageUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetDamageText(float DamageAmount, bool bIsCritical);

	UPROPERTY(BlueprintReadOnly)
	bool bIsDamageTaken = false;

	UPROPERTY(BlueprintReadOnly)
	bool bCriticalHit = false;
	
protected:
	UPROPERTY(meta=(BindWidgetOptional), BlueprintReadWrite)
	UTextBlock* DamageText = nullptr;
	
};
