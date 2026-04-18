// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPHUD.generated.h"

class UTPDamageUserWidget;
class UTPOverlayWidget;

UCLASS()
class TESTPROJECT_API ATPHUD : public AHUD
{
	GENERATED_BODY()
	
	
public:
	void InitHud();
	void ShowDamageNumber(float DamageAmount, FVector_NetQuantize WorldLocation, bool bIsCritical, bool bIsTaken);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPDamageUserWidget> DamageNumberWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPOverlayWidget> OverlayWidgetClass;
	
};
