#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CombatComponentData.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnAdd,
	ApplyOnRemove,
	ApplyOnBoth
};

USTRUCT(BlueprintType)
struct FEffectHandlingAbilities
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AbilityToActivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AbilityActivationDelay = 0.f;
};

USTRUCT(BlueprintType)
struct FEffectHandlingEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectApplicationDelay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToRemove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectRemovalDelay = 0.f;
};

USTRUCT(BlueprintType)
struct FEffectHandlingInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TriggerTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::ApplyOnAdd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEffectHandlingAbilities AbilitySettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEffectHandlingEffects EffectSettings;
};
