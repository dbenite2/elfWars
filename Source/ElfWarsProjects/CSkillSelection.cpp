// Fill out your copyright notice in the Description page of Project Settings.


#include "CSkillSelection.h"

void UCSkillSelection::AddSkill(const int& SkillToAdd) {
	if (SkillList.Num() < 4) {
		SkillList.Push(SkillToAdd);
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Skill list"), SkillList.Num());
	UE_LOG(LogTemp, Log, TEXT("log text: %d"), SkillList.Num());
}

TArray<int32> UCSkillSelection::GetSkillList() {
	return SkillList;
}

