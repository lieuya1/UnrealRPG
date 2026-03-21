#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "Weapon.generated.h"

UCLASS()
class TESTPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	// Attaches the weapon to the character at the provided attachment point
	UFUNCTION(BlueprintCallable)
	void AttachToCharacter(const FName& AttachmentPoint);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponSkeletalMesh;
};
