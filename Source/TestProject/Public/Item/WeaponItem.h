#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponItem.generated.h"

class AWeapon;

UCLASS()
class TESTPROJECT_API UWeaponItem : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWeapon> WeaponClass;
};
