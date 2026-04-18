#include "Tags/TPGameplayTags.h"
#include "GameplayTagsManager.h"

FTPGameplayTags FTPGameplayTags::GameplayTags;

void FTPGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("4"));
	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.5"), FString("5"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("RMB"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("LMB"));

	GameplayTags.State_Blocking = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Blocking"), FString("Blocking"));
	GameplayTags.State_Parrying = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Parrying"), FString("Parrying"));

	GameplayTags.Hit_Critical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Hit.Critical"), FString("Critical Hit"));
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
}
