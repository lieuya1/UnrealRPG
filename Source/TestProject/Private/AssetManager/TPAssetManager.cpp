#include "AssetManager/TPAssetManager.h"
#include "Tags/TPGameplayTags.h"
#include "AbilitySystemGlobals.h"

UTPAssetManager& UTPAssetManager::Get()
{
	check(GEngine);
	UTPAssetManager* TPAssetManager = Cast<UTPAssetManager>(GEngine->AssetManager);
	return *TPAssetManager;
}

void UTPAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FTPGameplayTags::InitializeNativeGameplayTags();
	// ✅ Make sure this is here too!
	UAbilitySystemGlobals::Get().InitGlobalData();
}



