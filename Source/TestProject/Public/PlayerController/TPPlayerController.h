#pragma once

#include "CoreMinimal.h"
#include <GameFramework/PlayerController.h>
#include "GameplayTagContainer.h"
#include "EnhancedInputComponent.h"
#include "TPPlayerController.generated.h"

class UTPAbilitySystemComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TESTPROJECT_API ATPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPPlayerController();

	UFUNCTION(Client, Reliable)
	void Client_ShowDamageNumber(float DamageAmount, FVector_NetQuantize WorldLocation, bool bIsCritical, bool bIsTaken);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& MoveActionValue);
	void Look(const FInputActionValue& LookActionValue);
	void AbilityAction(FGameplayTag ActionTag, bool bPressed);

	template<class TargetClass, typename TargetFunction>
	void BindTaggedInputActions(TMap<TObjectPtr<UInputAction>, FGameplayTag>& InputTagsMap, TargetClass* TargetObject, TargetFunction Callback);
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TMap<TObjectPtr<UInputAction>, FGameplayTag> InputTagsMap;

	UPROPERTY()
	TObjectPtr<UTPAbilitySystemComponent> ASC;
};

template <class TargetClass, typename TargetFunction>
void ATPPlayerController::BindTaggedInputActions(TMap<TObjectPtr<UInputAction>, FGameplayTag>& InputTagsMap,
	TargetClass* TargetObject, TargetFunction Callback)
{
	if (InputTagsMap.IsEmpty()) return;

	if (UEnhancedInputComponent* EnhanceInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		for (const TPair<TObjectPtr<UInputAction>, FGameplayTag>& Pair : InputTagsMap)
		{
			UInputAction* Action = Pair.Key;
			FGameplayTag Tag = Pair.Value;

			if (Action && Tag.IsValid())
			{
				// Pressed (Triggered)
				EnhanceInput->BindAction(Action, ETriggerEvent::Triggered, TargetObject, Callback, Tag, true);
				// Released (Completed)
				EnhanceInput->BindAction(Action, ETriggerEvent::Completed, TargetObject, Callback, Tag, false);
			}
		}
	}
}
