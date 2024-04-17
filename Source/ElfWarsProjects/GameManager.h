// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class ELFWARSPROJECTS_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor, BlueprintCallable)
	void SetSkillSet(const int PlayerIndex, const int SkillIndex);
	
	UFUNCTION(CallInEditor, BlueprintCallable)
	TArray<int32> GetSkillSet(const int& PlayerIndex);
	
	UFUNCTION(CallInEditor, BlueprintCallable)
	bool GetSelectionLock(const int PlayerIndex);

private:
	UPROPERTY(EditAnywhere, Blueprintable, Category="Skills")
	TArray<int32> SkillSetP1 {1,2,3,4};
	
	UPROPERTY(EditAnywhere, Blueprintable, Category="Skills")
	TArray<int32> SkillSetP2 {2,4,5,3};
	
	UPROPERTY(EditAnywhere, Blueprintable, Category="Selection complete")
	bool bSelectionLockP1;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Selection complete")
	bool bSelectionLockP2;
	
};
