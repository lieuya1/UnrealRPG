#include "Character/BaseCharacter.h"
#include "Component/CombatComponent.h"
#include "Component/TPAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");
	CombatComponent->SetIsReplicated(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABaseCharacter::AddAbilities() const
{
	if (!HasAuthority()) return;

	if (UTPAbilitySystemComponent* ASC = Cast<UTPAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AddAbilities(CommonAbilities);
		ASC->AddAbilities(DestroyerAbilities);
	}
}

void ABaseCharacter::PushActor_Implementation(const FVector& Direction, const float Strength)
{
	FVector NormalizedDirection = Direction.GetSafeNormal();
	FVector Offset = NormalizedDirection * Strength;
	FVector NewLocation = GetActorLocation() + Offset;

	SetActorLocation(NewLocation, true);
}

void ABaseCharacter::RotateActor_Implementation(const FRotator& Rotation)
{
	if (HasAuthority())
	{
		MulticastRotateActor(Rotation);
	}
}

void ABaseCharacter::MulticastRotateActor_Implementation(const FRotator& Rotation)
{
	SetActorRotation(Rotation, ETeleportType::None);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && CombatComponent)
	{
		CombatComponent->SpawnAndConfigureWeapons();
	}
}

void ABaseCharacter::InitializeDefaultAttributes()
{
	ApplyEffectToTarget(SecondaryAttributes, 1.f);
	ApplyEffectToTarget(MainAttributes, 1.f);
	ApplyEffectToTarget(DefaultAttributes, 1.f);
}

void ABaseCharacter::ApplyEffectToTarget(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC || !GameplayEffectClass) return;
    
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}


