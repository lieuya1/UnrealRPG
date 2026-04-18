#include "Component/CombatComponent.h"
#include <Net/UnrealNetwork.h>
#include "Actor/CollisionActor.h"
#include "Item/WeaponItem.h"
#include "Actor/Weapon.h"
#include "AnimNotify/CollisionActivationData.h"
#include "Character/BaseCharacter.h"
#include "Component/TPAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, MainHandWeapon);
	DOREPLIFETIME(UCombatComponent, OffHandWeapon);
}

bool UCombatComponent::SpawnAndConfigureWeapons()
{
	if (WeaponItemClasses.IsEmpty() || !GetOwner() || !GetOwner()->HasAuthority()) return false;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const TSubclassOf<UWeaponItem>& WeaponItemClass : WeaponItemClasses)
	{
		if (!IsValid(WeaponItemClass)) continue;

		if (UWeaponItem* WeaponItem = NewObject<UWeaponItem>(this, WeaponItemClass))
		{
			if (AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponItem->WeaponClass, SpawnParams))
			{
				Weapon->SetReplicates(true);
				HandleEquipWeapon(Weapon);
			}
		}
	}
	return true;
}

ACollisionActor* UCombatComponent::SpawnAndActivateCollision(const FCollisionActivationData& CollisionData)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return nullptr;
	if (CollisionData.EffectClasses.IsEmpty() && !CollisionData.DamageEffectClasses) return nullptr;

	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		FName SocketName = FCollisionActivationData::GetSocketName(CollisionData);
		FTransform SpawnTransform = OwnerCharacter->GetMesh()->GetSocketTransform(SocketName);
		
		ACollisionActor* CollisionActor = GetWorld()->SpawnActorDeferred<ACollisionActor>(
			CollisionActorClass,
			SpawnTransform,
			OwnerCharacter,
			OwnerCharacter,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
			);

		if (CollisionActor)
		{
			CollisionActor->SetOwner(OwnerCharacter);
			CollisionActor->Initialize(CollisionData);
			CollisionActor->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			CollisionActor->FinishSpawning(SpawnTransform);
			CollisionActor->SetActorRelativeLocation(CollisionData.ShapeSettings.RelativeOffset);

			return CollisionActor;
		}
	}
	
	return nullptr;
}

void UCombatComponent::Multicast_BroadcastTagChanged_Implementation(FGameplayTag Tag, int32 NewCount)
{
	OnTagChanged.Broadcast(Tag, NewCount);
}

void UCombatComponent::BindTagChangedCallbacks()
{
	if (UTPAbilitySystemComponent* ASC = Cast<UTPAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		for (const FEffectHandlingInfo& TagToBind : TagBindings)
		{
			ASC->RegisterGameplayTagEvent(TagToBind.TriggerTag, EGameplayTagEventType::AnyCountChange).AddUObject(this, &UCombatComponent::HandleTagsChanged);
		}
	}
}

void UCombatComponent::HandleTagsChanged(FGameplayTag Tag, int32 NewCount)
{
	if (UTPAbilitySystemComponent* ASC = Cast<UTPAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		bool bTagAdded = NewCount > 0;

		for (FEffectHandlingInfo TagBinding : TagBindings)
		{
			if (!Tag.MatchesTag(TagBinding.TriggerTag)) continue;

			switch (TagBinding.ApplicationPolicy)
			{
			case EEffectApplicationPolicy::ApplyOnAdd:
				if (!bTagAdded) continue;
				RemoveEffects(TagBinding, Tag, ASC);
				ApplyEffects(TagBinding, Tag, ASC);
				ActivateAbilities(TagBinding, Tag, ASC);
				break;
			case EEffectApplicationPolicy::ApplyOnRemove:
				if (bTagAdded) continue;
				RemoveEffects(TagBinding, Tag, ASC);
				ApplyEffects(TagBinding, Tag, ASC);
				ActivateAbilities(TagBinding, Tag, ASC);
				break;
			case EEffectApplicationPolicy::ApplyOnBoth:
				RemoveEffects(TagBinding, Tag, ASC);
				ApplyEffects(TagBinding, Tag, ASC);
				ActivateAbilities(TagBinding, Tag, ASC);
				break;
			}
		}

		Multicast_BroadcastTagChanged(Tag, NewCount);
	}
}

void UCombatComponent::ActivateAbilities(const FEffectHandlingInfo& Info, const FGameplayTag Tag,
	UTPAbilitySystemComponent* ASC)
{
	if (Info.AbilitySettings.AbilityToActivate.IsValid())
	{
		auto ActivateAbilities = [ASC, Info]
		{
			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(Info.AbilitySettings.AbilityToActivate));
		};

		FTimerHandle& Handle = TimerHandlesAbilities.FindOrAdd(Tag);
		ExecuteWithDelay(ActivateAbilities, Info.AbilitySettings.AbilityActivationDelay, Handle);
	}
}

void UCombatComponent::RemoveEffects(const FEffectHandlingInfo& Info, const FGameplayTag Tag,
	UTPAbilitySystemComponent* ASC)
{
	if (Info.EffectSettings.EffectsToRemove.Num() > 0)
	{
		auto RemoveEffects = [ASC, Info]
		{
			for (const TSubclassOf<UGameplayEffect>& Effect: Info.EffectSettings.EffectsToRemove)
			{
				if (Effect && ASC)
				{
					FGameplayEffectQuery Query;
					Query.EffectDefinition = Effect;
					ASC->RemoveActiveEffects(Query);
				}
			}
		};

		FTimerHandle& Handle = TimerHandlesRemove.FindOrAdd(Tag);
		ExecuteWithDelay(RemoveEffects, Info.EffectSettings.EffectRemovalDelay, Handle);
	}
}

void UCombatComponent::ApplyEffects(const FEffectHandlingInfo& Info, const FGameplayTag Tag,
	UTPAbilitySystemComponent* ASC)
{
	if (Info.EffectSettings.EffectsToApply.Num() > 0)
	{
		auto ApplyEffects = [this, ASC, Info]
		{
			for (const TSubclassOf<UGameplayEffect>& Effect: Info.EffectSettings.EffectsToApply)
			{
				if (Effect && ASC)
				{
					FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
					EffectContext.AddSourceObject(GetOwner());
					FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, 1.f, EffectContext);
					if (SpecHandle.IsValid())
					{
						ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
		};
		
		FTimerHandle& Handle = TimerHandlesApply.FindOrAdd(Tag);
		ExecuteWithDelay(ApplyEffects, Info.EffectSettings.EffectApplicationDelay, Handle);
	}
}

template <typename Func>
void UCombatComponent::ExecuteWithDelay(Func InFunction, float Delay, FTimerHandle& TimerHandle)
{
	if (Delay == 0) { InFunction(); return; }

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda(InFunction);
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Delay, false);
	}
}

void UCombatComponent::HandleEquipWeapon(AWeapon* Weapon)
{
	if (!IsValid(Weapon)) return;

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		MainHandWeapon = Weapon;
	}

	Weapon->AttachToCharacter(FName("Weapon_R"));
}

void UCombatComponent::OnRep_MainHandWeapon()
{
	if (MainHandWeapon)
	{
		HandleEquipWeapon(MainHandWeapon);
	}
}

void UCombatComponent::OnRep_OffHandWeapon()
{
	if (OffHandWeapon)
	{
		HandleEquipWeapon(OffHandWeapon);
	}
}
