#include "Actor/CollisionActor.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <AbilitySystemComponent.h>
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/HitInterface.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "Ability/TPGameplayAbility.h"
#include "Tags/TPGameplayTags.h"

ACollisionActor::ACollisionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	RootComponent = DefaultRootComponent;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	SetupCollisionComponent(CollisionBox);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	SetupCollisionComponent(CollisionSphere);

	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void ACollisionActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollisionActor, CollisionBox);
	DOREPLIFETIME(ACollisionActor, CollisionSphere);
	DOREPLIFETIME(ACollisionActor, CachedCollisionData);
}

void ACollisionActor::Initialize(const FCollisionActivationData& CollisionData)
{
	if (!HasAuthority()) return;

	IgnoreActors.Empty();
	CachedCollisionData = CollisionData;
	
	SelectCollisionComponent();

	if (!ActiveCollisionComponent) return;
	ActiveCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActiveCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnOverlapBegin);

	if (CollisionData.bShowDebugCollision) MulticastShowDebugCollision();

	PrepEffectSpecs();
}

void ACollisionActor::OnRep_CollisionActivationData()
{
	SelectCollisionComponent();
}

void ACollisionActor::SelectCollisionComponent()
{
	switch (CachedCollisionData.ShapeSettings.CollisionShapeType)
	{
	case ECollisionShapeTypes::Sphere:
		ActiveCollisionComponent = CollisionSphere;
		CollisionSphere->SetSphereRadius(CachedCollisionData.ShapeSettings.SphereRadius);
		CollisionSphere->SetLineThickness(5.f);
		break;
	case ECollisionShapeTypes::Box:
		ActiveCollisionComponent = CollisionBox;
		CollisionBox->SetBoxExtent(CachedCollisionData.ShapeSettings.BoxExtent);
		CollisionBox->SetLineThickness(5.f);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid Collision Shape Type"));
		break;
	}
}

void ACollisionActor::SetupCollisionComponent(UPrimitiveComponent* Component)
{
	Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Component->SetCollisionObjectType(ECC_WorldDynamic);
	Component->SetCollisionResponseToAllChannels(ECR_Ignore);
	Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Component->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Component->SetGenerateOverlapEvents(true);
}

void ACollisionActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || OtherActor->GetOwner() == GetOwner() || IgnoreActors.Contains(OtherActor)) return;
	
	BlockedAndParriedAttack(OtherActor);
	HandleRotation(OtherActor);
	HandleMovement(OtherActor);
	ApplyGameplayEffects(OtherActor);

	IgnoreActors.Add(OtherActor);
}

void ACollisionActor::BlockedAndParriedAttack(AActor* OtherActor)
{	
	if (!OtherActor) return;

	auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!ASC) return;

	AActor* Attacker = GetOwner();
	if (!Attacker) return;

	const FVector ToAttacker = (Attacker->GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
	const FVector DefenderForward = OtherActor->GetActorForwardVector().GetSafeNormal();

	const float DegreeThreshold = FMath::RadiansToDegrees(acosf(FVector::DotProduct(DefenderForward, ToAttacker)));

	UE_LOG(LogTemp, Warning, TEXT("DegreeThreshold: %f"), DegreeThreshold);

	if (DegreeThreshold < 70.f)
	{
		ASC->HasMatchingGameplayTag(FTPGameplayTags::Get().State_Blocking) ? bBlocking = true : bBlocking = false;
		ASC->HasMatchingGameplayTag(FTPGameplayTags::Get().State_Parrying) ? bParrying = true : bParrying = false;
	}
}

void ACollisionActor::HandleMovement(AActor* OtherActor)
{
	if (!OtherActor) return;

	const auto& MS = CachedCollisionData.MovementSettings;

	if (MS.bShouldMoveActor)
	{
		// Determine which actor to move
		AActor* ActorToMove = nullptr;
		if (MS.ActorToMove == EMoveTarget::Owner)
		{
			ActorToMove = GetOwner();
		}
		if (MS.ActorToMove == EMoveTarget::HitActor)
		{
			ActorToMove = OtherActor;
		}
		
		if (ActorToMove && ActorToMove->Implements<UHitInterface>())
		{
			FVector InstigatorForward = GetOwner()->GetActorForwardVector();
			InstigatorForward.Z = 0.f;
			InstigatorForward.Normalize();

			FVector PushDirection = InstigatorForward;
			if (MS.MoveDirection == EMoveDirection::Forward)
			{
				PushDirection *= -1;
			}

			IHitInterface::Execute_PushActor(ActorToMove, PushDirection, MS.MoveDistance);
		}
	}
}

void ACollisionActor::HandleRotation(AActor* OtherActor)
{
	if (!OtherActor || bBlocking || bParrying) return;
	if (!CachedCollisionData.bRotateActor) return;

	const FVector InstigatorLocation = GetOwner()->GetActorLocation();
	const FVector TargetLocation = OtherActor->GetActorLocation();

	FVector Direction = InstigatorLocation - TargetLocation;
	Direction.Z = 0.f;
	Direction.Normalize();
	FRotator DesiredRotation = Direction.Rotation();

	if (!HasAuthority()) return;
	if (!OtherActor->Implements<UHitInterface>()) return;
	IHitInterface::Execute_RotateActor(OtherActor, DesiredRotation);
}

void ACollisionActor::PrepEffectSpecs()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return;

	UTPGameplayAbility* Ability = Cast<UTPGameplayAbility>(ASC->GetAnimatingAbility());
	if (!Ability) return;

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetOwner());
	EffectContext.AddInstigator(GetOwner(), this);

	// ✅ Only add non-damage effects here
	for (const TSubclassOf<UGameplayEffect>& EffectClass : CachedCollisionData.EffectClasses)
	{
		if (!IsValid(EffectClass)) continue;

		// ✅ Skip if this effect is the same as DamageEffectClasses to avoid duplicates
		if (EffectClass == CachedCollisionData.DamageEffectClasses) continue;

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
			EffectClass, Ability->GetAbilityLevel(), EffectContext);
		CachedEffectSpecs.Add(SpecHandle);
	}

	// ✅ Add damage effect only once separately
	if (CachedCollisionData.DamageEffectClasses)
	{
		float Damage = CachedCollisionData.Damage;
        
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
			CachedCollisionData.DamageEffectClasses, 
			Ability->GetAbilityLevel(), 
			EffectContext);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle, FTPGameplayTags::Get().Damage, Damage);
		CachedEffectSpecs.Add(SpecHandle);
	}
}

void ACollisionActor::ApplyGameplayEffects(AActor* OtherActor)
{
	if (!OtherActor || bBlocking || bParrying) return;

	if (UAbilitySystemComponent* ASC = 
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		for (const FGameplayEffectSpecHandle& EffectSpecHandle : CachedEffectSpecs)
		{
			if (!EffectSpecHandle.IsValid()) continue;

			// ✅ Set origin location on each spec before applying
			FGameplayEffectContextHandle Context = EffectSpecHandle.Data->GetContext();
			Context.AddOrigin(GetActorLocation());
			EffectSpecHandle.Data->SetContext(Context);

			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}

void ACollisionActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ActiveCollisionComponent)
	{
		ActiveCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ActiveCollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ACollisionActor::OnOverlapBegin);
	}
}

void ACollisionActor::MulticastShowDebugCollision_Implementation()
{
	if (GetOwner() && !GetOwner()->HasLocalNetOwner()) return;

	if (ActiveCollisionComponent)
	{
		ActiveCollisionComponent->SetHiddenInGame(false);
	}
}


