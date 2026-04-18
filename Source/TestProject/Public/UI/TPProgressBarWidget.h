// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPProgressBarWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged,float, NewValue);

struct FOnAttributeChangeData;
class UProgressBar;
class UTextBlock;
class ACharacter;
class UTPAttributeSet;

UCLASS()
class TESTPROJECT_API UTPProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeWidget(ACharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	void SetProgressBarValues(float Current, float Max);

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetInitialized();

	UFUNCTION(BlueprintCallable)
	void BroadCastInitialValues(UTPAttributeSet* AS);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnEnergyChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnMaxEnergyChanged;
	

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void EnergyChanged(const FOnAttributeChangeData& Data) const;
	void MaxEnergyChanged(const FOnAttributeChangeData& Data) const;
	
	UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
	UProgressBar* ProgressBar = nullptr;

	UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
	UTextBlock* CurrentText = nullptr;

	UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
	UTextBlock* MaxText = nullptr;
	
};
