// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/BaseCharacter.h"
#include "TPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UTPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	//cache reference
	UPROPERTY()
	ABaseCharacter* BaseCharacter = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

	//movement properties
	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true" ))
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true" ))
	bool bIsAccelerating = false;

	// Rotation  properties
	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true" ))
	FRotator AimRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true" ))
	FRotator MovementRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Anim|Movement", meta = (AllowPrivateAccess="true" ))
	float MovementOffsetYaw;
};
