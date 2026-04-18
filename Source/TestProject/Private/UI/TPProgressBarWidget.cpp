// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPProgressBarWidget.h"
#include <Components/TextBlock.h>
#include "AttributeSet/TPAttributeSet.h"
#include "Character/BaseCharacter.h"
#include "Components/ProgressBar.h"

void UTPProgressBarWidget::InitializeWidget(ACharacter* Character)
{
	if (!Character) return;
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Character);

	auto ASC = BaseCharacter->GetAbilitySystemComponent();
	auto AS = Cast<UTPAttributeSet>(BaseCharacter->GetAttributeSet());
	if (!ASC || !AS) return;

	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &UTPProgressBarWidget::HealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &UTPProgressBarWidget::MaxHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetEnergyAttribute()).AddUObject(this, &UTPProgressBarWidget::EnergyChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxEnergyAttribute()).AddUObject(this, &UTPProgressBarWidget::MaxEnergyChanged);

	WidgetInitialized();
	BroadCastInitialValues(AS);
}

void UTPProgressBarWidget::SetProgressBarValues(float Current, float Max)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(Current / Max);
	}
	
	if (CurrentText)
	{
		CurrentText->SetText(FText::AsNumber(FMath::RoundToInt(Current)));
	}

	if (MaxText)
	{
		MaxText->SetText(FText::AsNumber(FMath::RoundToInt(Max)));
	}
}

void UTPProgressBarWidget::BroadCastInitialValues(UTPAttributeSet* AS)
{
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	OnEnergyChanged.Broadcast(AS->GetEnergy());
	OnMaxEnergyChanged.Broadcast(AS->GetMaxEnergy());
}

void UTPProgressBarWidget::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UTPProgressBarWidget::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UTPProgressBarWidget::EnergyChanged(const FOnAttributeChangeData& Data) const
{
	OnEnergyChanged.Broadcast(Data.NewValue);
}

void UTPProgressBarWidget::MaxEnergyChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxEnergyChanged.Broadcast(Data.NewValue);
}
