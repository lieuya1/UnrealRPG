#pragma once

#include "CoreMinimal.h"
#include <GameFramework/PlayerController.h>
#include "TPPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TESTPROJECT_API ATPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& MoveActionValue);
	void Look(const FInputActionValue& LookActionValue);
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
};
