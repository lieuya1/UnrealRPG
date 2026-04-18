// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UTPOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OverlayInitialized();
	
};
