// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProject/Public/AttributeSet/TPAttributeSet.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "PlayerController/TPPlayerController.h"
#include "Tags/TPGameplayTags.h"

void UTPAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}

void UTPAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
	if (FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		if (Attribute == GetMovementSpeedAttribute())
		{
			if (UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
			{
				CharacterMovement->MaxWalkSpeed = NewValue;
			}
		}
	}
}

void UTPAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));
	}
	if (Attribute == GetIncomingDamageAttribute())
	{
		if (GetIncomingDamage() <= 0.f) return;
		const float Damage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		FGameplayTagContainer GrantedTags;
		Data.EffectSpec.GetAllGrantedTags(GrantedTags);

		bool bCritical = GrantedTags.HasTagExact(FTPGameplayTags::Get().Hit_Critical);

		ATPPlayerController* SourcePlayerController = nullptr;
		ATPPlayerController* TargetPlayerController = nullptr;
		FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
		UAbilitySystemComponent* ASC = Context.GetOriginalInstigatorAbilitySystemComponent();
		if (ASC)
		{
			SourcePlayerController = Cast<ATPPlayerController>(ASC->AbilityActorInfo->PlayerController.Get());
			TargetPlayerController = Cast<ATPPlayerController>(Data.Target.AbilityActorInfo->PlayerController.Get());
		}

		FVector WorldLocation;
		if (Context.HasOrigin())
		{
			WorldLocation = Context.GetOrigin();
		}

		if (SourcePlayerController)
		{
			// Show Damage Widget Number
			SourcePlayerController->Client_ShowDamageNumber(Damage, WorldLocation, bCritical, false);
		}

		if (TargetPlayerController)
		{
			// Show Damage Widget Number
			TargetPlayerController->Client_ShowDamageNumber(Damage, WorldLocation, bCritical, true);
		}

		if (Damage > 0.f)
		{
			const float CurrentHealth = GetHealth();
			const float NewHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, GetMaxHealth());

			GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("Health: %f"), NewHealth));
			if (bCritical)
			{
				GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Yellow, FString::Printf(TEXT("Critical Hit!")));
			}
			SetHealth(NewHealth);
		}
	}
}

void UTPAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, Health, OldHealth);
}

void UTPAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, MaxHealth, OldMaxHealth);
}

void UTPAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, Energy, OldEnergy);
}

void UTPAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UTPAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, PhysicalAttack, OldPhysicalAttack);
}

void UTPAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UTPAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, CriticalChance, OldCriticalChance);
}

void UTPAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, CriticalDamage, OldCriticalDamage);
}

void UTPAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, Vitality, OldVitality);
}

void UTPAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, Intelligence, OldIntelligence);
}

void UTPAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPAttributeSet, MovementSpeed, OldMovementSpeed);
}
