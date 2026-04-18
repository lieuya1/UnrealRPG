#include "AnimNotify/PLCollisionNotifyState.h"
#include "Actor/CollisionActor.h"
#include "Character/BaseCharacter.h"
#include "Component/CombatComponent.h"

void UPLCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
										  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp) return;

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		if (Character->HasAuthority())
		{
			if (UCombatComponent* CombatComponent = Character->GetCombatComponent())
			{
				CollisionActor = CombatComponent->SpawnAndActivateCollision(CollisionData);
			}
		}
	}
}

void UPLCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		if (Character->HasAuthority())
		{
			if (CollisionActor.IsValid())
			{
				CollisionActor->Destroy();
			}
		}
	}
}


