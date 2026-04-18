#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CollisionActivationData.h"
#include "PLCollisionNotifyState.generated.h"

class UGameplayEffect;
class ACollisionActor;

UCLASS()
class TESTPROJECT_API UPLCollisionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
	FCollisionActivationData CollisionData;
	
private:
	TWeakObjectPtr<ACollisionActor> CollisionActor;
};
