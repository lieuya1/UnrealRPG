#pragma once

#include "CoreMinimal.h"
#include "CollisionActivationData.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECollisionSocket : uint8
{
	MainHandWeapon,
	OffHandWeapon,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightShoulder,
	LeftShoulder,
	CustomSocket
};

USTRUCT(BlueprintType)
struct FCollisionSocketSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ECollisionSocket CollisionSocket = ECollisionSocket::CustomSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta=(EditCondition = "CollisionSocket == ECollisionSocket::CustomSocket", EditConditionHides))
	FName CustomSocketName = NAME_None;
	
};

UENUM(BlueprintType)
enum class ECollisionShapeTypes : uint8
{
	Sphere,
	Box
};

USTRUCT(BlueprintType)
struct FCollisionShapeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ECollisionShapeTypes CollisionShapeType = ECollisionShapeTypes::Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta=(EditCondition = "CollisionShapeType == ECollisionShapeTypes::Box", EditConditionHides))
	FVector BoxExtent = FVector(20.f, 20.f, 20.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta=(EditCondition = "CollisionShapeType == ECollisionShapeTypes::Sphere", EditConditionHides))
	float SphereRadius = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FVector RelativeOffset = FVector(0.f, 0.f, 0.f);
};

UENUM(BlueprintType)
enum class EMoveTarget : uint8
{
	Owner,
	HitActor
};

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Forward,
	Backward
};

USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bShouldMoveActor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(EditCondition = "bShouldMoveActor", EditConditionHides))
	EMoveTarget ActorToMove = EMoveTarget::HitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(EditCondition = "bShouldMoveActor", EditConditionHides))
	EMoveDirection MoveDirection = EMoveDirection::Backward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(EditCondition = "bShouldMoveActor", EditConditionHides))
	float MoveDistance = 25.f;
};

// --------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FCollisionActivationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FCollisionSocketSettings SocketSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FCollisionShapeSettings ShapeSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TSubclassOf<UGameplayEffect> DamageEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FMovementSettings MovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bRotateActor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugCollision = false;

	// -------------------------------------------------------------------------------------------- //

	static FName GetSocketName(const FCollisionActivationData& Data)
	{
		if (Data.SocketSettings.CollisionSocket == ECollisionSocket::CustomSocket)
		{
			if (Data.SocketSettings.CustomSocketName.IsNone())
			{
				UE_LOG(LogTemp, Error, TEXT("CollisionActivationData: Custom Socket chosen but CustomSocketName is None"));
			}
			return Data.SocketSettings.CustomSocketName;
		}

		static const TMap<ECollisionSocket, FName> SocketMap = {
			{ECollisionSocket::MainHandWeapon, TEXT("Weapon_R")},
			{ECollisionSocket::OffHandWeapon, TEXT("Weapon_L")},
			{ECollisionSocket::RightFoot, TEXT("RightFootSocket")},
			{ECollisionSocket::LeftFoot, TEXT("LeftFootSocket")},
			{ECollisionSocket::RightHand, TEXT("RightHandSocket")},
			{ECollisionSocket::LeftHand, TEXT("LeftHandSocket")},
			{ECollisionSocket::RightShoulder, TEXT("RightShoulderSocket")},
			{ECollisionSocket::LeftShoulder, TEXT("LeftShoulderSocket")}
		};

		if (const FName* FoundSocket = SocketMap.Find(Data.SocketSettings.CollisionSocket))
		{
			return *FoundSocket;
		}
		return NAME_None;
	}
};

