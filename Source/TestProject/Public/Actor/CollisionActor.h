#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "AnimNotify/CollisionActivationData.h"
#include "CollisionActor.generated.h"

struct FGameplayEffectSpecHandle;
struct FCollisionActivationData;
class UGameplayEffect;
class USphereComponent;
class UBoxComponent;


UCLASS()
class TESTPROJECT_API ACollisionActor : public AActor
{
	GENERATED_BODY()

public:
	ACollisionActor();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void Initialize(const FCollisionActivationData& CollisionData);

	UPROPERTY()
	USceneComponent* DefaultRootComponent;

	bool bBlocking = false;
	bool bParrying = false;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowDebugCollision();
	
private:
	void SelectCollisionComponent();
	void SetupCollisionComponent(UPrimitiveComponent* Component);
	void HandleMovement(AActor* OtherActor);
	void HandleRotation(AActor* OtherActor);
	void PrepEffectSpecs();
	void ApplyGameplayEffects(AActor* OtherActor);
	void BlockedAndParriedAttack(AActor* OtherActor);
	
	UPROPERTY(Replicated)
	USphereComponent* CollisionSphere;

	UPROPERTY(Replicated)
	UBoxComponent* CollisionBox;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	UPROPERTY(Transient)
	UPrimitiveComponent* ActiveCollisionComponent;

	UPROPERTY()
	TArray<FGameplayEffectSpecHandle> CachedEffectSpecs;

	UPROPERTY(ReplicatedUsing = OnRep_CollisionActivationData)
	FCollisionActivationData CachedCollisionData;
	
	UFUNCTION()
	void OnRep_CollisionActivationData();
};
