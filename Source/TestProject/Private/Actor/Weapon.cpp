#include "Actor/Weapon.h"
#include "Character/BaseCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponSkeletalMesh"));
	SetRootComponent(WeaponSkeletalMesh);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponSkeletalMesh->SetGenerateOverlapEvents(true);
	WeaponSkeletalMesh->SetHiddenInGame(true);
}

void AWeapon::AttachToCharacter(const FName& AttachmentPoint)
{
	if (AttachmentPoint == NAME_None) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (!Character) return;

	WeaponSkeletalMesh->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachmentPoint);
	WeaponSkeletalMesh->SetHiddenInGame(false);
}




