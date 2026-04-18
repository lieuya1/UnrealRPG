#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Component/TPAbilitySystemComponent.h"
#include "TPAssetManager.generated.h"

UCLASS()
class TESTPROJECT_API UTPAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UTPAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};