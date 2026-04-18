#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "GameplayTagContainer.h"
#include "CombatComponentData.h"
#include "CombatComponent.generated.h"

struct FCollisionActivationData;
class ACollisionActor;
class UWeaponItem;
class AWeapon;
class UGameplayEffect;
class UTPAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagChanged, const FGameplayTag&, Tag, int32, NewCount);

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

	UFUNCTION(BlueprintCallable)
	ACollisionActor* SpawnAndActivateCollision(const FCollisionActivationData& CollisionData);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastTagChanged(FGameplayTag Tag, int32 NewCount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Config|Weapons")
	TArray<TSubclassOf<UWeaponItem>> WeaponItemClasses;

	void BindTagChangedCallbacks();

	UPROPERTY(EditAnywhere)
	TArray<FEffectHandlingInfo> TagBindings;

	UPROPERTY()
	FOnTagChanged OnTagChanged;


private:
	void ActivateAbilities(const FEffectHandlingInfo& Info, const FGameplayTag Tag, UTPAbilitySystemComponent* ASC);
	void RemoveEffects(const FEffectHandlingInfo& Info, const FGameplayTag Tag, UTPAbilitySystemComponent* ASC);
	void ApplyEffects(const FEffectHandlingInfo& Info, const FGameplayTag Tag, UTPAbilitySystemComponent* ASC);

	TMap<FGameplayTag, FTimerHandle> TimerHandlesAbilities;
	TMap<FGameplayTag, FTimerHandle> TimerHandlesRemove;
	TMap<FGameplayTag, FTimerHandle> TimerHandlesApply;

	template<typename Func>
	void ExecuteWithDelay(Func InFunction, float Delay, FTimerHandle& TimerHandle);
	
	void HandleEquipWeapon(AWeapon* Weapon);

	void HandleTagsChanged(FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MainHandWeapon, VisibleAnywhere)
	TObjectPtr<AWeapon> MainHandWeapon;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_OffHandWeapon, VisibleAnywhere)
	TObjectPtr<AWeapon> OffHandWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Config|Collision")
	TSubclassOf<ACollisionActor> CollisionActorClass;

	UFUNCTION()
	void OnRep_MainHandWeapon();
	
	UFUNCTION()
	void OnRep_OffHandWeapon();
};
