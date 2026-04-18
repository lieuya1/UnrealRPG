// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Component/CombatComponent.h"
#include "TPAnimInstance.generated.h"

class UCharacterMovementComponent;
class ABaseCharacter;

UCLASS()
class TESTPROJECT_API UTPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetCombatComponent(UCombatComponent* InCombatComponent) { CombatComponent = InCombatComponent; }

	void BindTagChanged();

private:
	UFUNCTION()
	void HandleStateTagChanged(const FGameplayTag& Tag, int32 NewCount);
	
	// Cached References
	UPROPERTY()
	ABaseCharacter* BaseCharacter = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY()
	UCombatComponent* CombatComponent = nullptr;

	UPROPERTY()
	UTPAbilitySystemComponent* AbilitySystemComponent = nullptr;

	// Movement Properties
	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true"))
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true"))
	bool bIsAccelerating = false;

	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true"))
	bool bIsBlocking;

	// Rotation Properties
	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true"))
	FRotator AimRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true"))
	FRotator MovementRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true"))
	float MovementOffsetYaw;
};
