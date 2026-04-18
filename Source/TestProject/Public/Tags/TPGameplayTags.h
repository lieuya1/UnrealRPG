#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FTPGameplayTags
{
public:
	static const FTPGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_LMB;

	FGameplayTag State_Blocking;
	FGameplayTag State_Parrying;

	FGameplayTag Hit_Critical;

	FGameplayTag Damage;
	
private:
	static FTPGameplayTags GameplayTags;
};
