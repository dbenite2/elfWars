// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSkillSelection.generated.h"

/**
 * 
 */
UCLASS()
class ELFWARSPROJECTS_API UCSkillSelection : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, Blueprintable, Category="Skill Selector")
	TArray<int32> SkillList;

	UFUNCTION(CallInEditor, BlueprintCallable)
	void AddSkill(const int& SkillToAdd);

	UFUNCTION(CallInEditor, BlueprintCallable)
	TArray<int32> GetSkillList();
};
