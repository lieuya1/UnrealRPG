#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameLiftServerSDK.h"
#include "TPGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameServerLog, Log, All);

UCLASS()
class TESTPROJECT_API ATPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATPGameMode();

protected:
	virtual void BeginPlay() override;

private:
	// Process Parameters needs to remain in scope for the lifetime of the app
	FProcessParameters m_params;

	void InitGameLift();
};
