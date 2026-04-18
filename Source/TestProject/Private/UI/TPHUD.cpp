// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPHUD.h"
#include "UI/TPDamageUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/TPOverlayWidget.h"

void ATPHUD::InitHud()
{
	if (!OverlayWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("OverlayWidgetClass is not set in the ATPHUD"));
		return;
	}

	UTPOverlayWidget* OverlayWidget = CreateWidget<UTPOverlayWidget>(GetWorld(), OverlayWidgetClass);
	if (OverlayWidget)
	{
		OverlayWidget->OverlayInitialized();
		OverlayWidget->SetVisibility(ESlateVisibility::Visible);
		OverlayWidget->AddToViewport();
	}
}

void ATPHUD::ShowDamageNumber(float DamageAmount, FVector_NetQuantize WorldLocation, bool bIsCritical, bool bIsTaken)
{
	if (!DamageNumberWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageNumberWidgetClass is not set in the ATPHUD!"));
	}

	APlayerController* PC = PlayerOwner;
	if (!PC)
	{
		PC = UGameplayStatics::GetPlayerController(this, 0);
	}
	if (!PC) return;

	FVector2D ScreenPosition;
	const bool bProjectedToScreen = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, WorldLocation, ScreenPosition, true);

	if (!bProjectedToScreen) return;

	UTPDamageUserWidget* DamageWidget = CreateWidget<UTPDamageUserWidget>(GetWorld(), DamageNumberWidgetClass);
	if (DamageWidget)
	{
		DamageWidget->bIsDamageTaken = bIsTaken;
		DamageWidget->SetDamageText(DamageAmount, bIsCritical);
		DamageWidget->SetVisibility(ESlateVisibility::Visible);
		DamageWidget->AddToViewport(0);
		DamageWidget->SetPositionInViewport(ScreenPosition, false);
	}
}
