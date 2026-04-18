#pragma once

#include <AbilitySystemInterface.h>
#include "CoreMinimal.h"
#include "Ability/TPGameplayAbility.h"
#include "Interface/HitInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCombatComponent;
class UAbilitySystemComponent;

UCLASS()
class TESTPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystemComponent;}
	UAttributeSet* GetAttributeSet() const {return AttributeSet; }

	UCombatComponent* GetCombatComponent() const { return CombatComponent;}
	
	void AddAbilities() const;

	virtual void PushActor_Implementation(const FVector& Direction, const float Strength) override;
	virtual void RotateActor_Implementation(const FRotator& Rotation) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRotateActor(const FRotator& Rotation);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CharacterLevel = 1.f;

protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes();

	void ApplyEffectToTarget(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat|Config|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat|Config|Attributes")
	TSubclassOf<UGameplayEffect> MainAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat|Config|Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Combat|Config|Abilities")
	TArray<TSubclassOf<UTPGameplayAbility>> CommonAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat|Config|Abilities")
	TArray<TSubclassOf<UTPGameplayAbility>> DestroyerAbilities;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess=true))
	TObjectPtr<UCombatComponent> CombatComponent;
};
