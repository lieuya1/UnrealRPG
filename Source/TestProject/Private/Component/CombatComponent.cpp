#include "Component/CombatComponent.h"
#include <Net/UnrealNetwork.h>
#include "Item/WeaponItem.h"
#include "Actor/Weapon.h"

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
