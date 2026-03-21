#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "CombatComponent.generated.h"

class UWeaponItem;
class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTPROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Weapon Spawning & Equipping
	UFUNCTION(BlueprintCallable)
	bool SpawnAndConfigureWeapons();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Config|Weapons")
	TArray<TSubclassOf<UWeaponItem>> WeaponItemClasses;

private:
	void HandleEquipWeapon(AWeapon* Weapon);
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MainHandWeapon, VisibleAnywhere)
	TObjectPtr<AWeapon> MainHandWeapon;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_OffHandWeapon, VisibleAnywhere)
	TObjectPtr<AWeapon> OffHandWeapon;

	UFUNCTION()
	void OnRep_MainHandWeapon();

	UFUNCTION()
	void OnRep_OffHandWeapon();
};
