// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPDamageUserWidget.h"
#include "Components/TextBlock.h"

void UTPDamageUserWidget::SetDamageText(float DamageAmount, bool bIsCritical)
{
	if (DamageText)
	{
		if (bIsDamageTaken)
		{
			DamageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		}
		bCriticalHit = bIsCritical;
		DamageText->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));
	}
}
