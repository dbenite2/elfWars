// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

TArray<int32> UGameManager::GetSkillSet(const int& PlayerIndex) {
	UE_LOG(LogTemp, Log, TEXT("PlayerIndex: %d"), PlayerIndex);
	return PlayerIndex == 0 ? SkillSetP1 : SkillSetP2;
}

void UGameManager::SetSkillSet(const int PlayerIndex, const int SkillIndex) {
	UE_LOG(LogTemp, Log, TEXT("Player Index: %d, Skill Index: %d"), PlayerIndex, SkillIndex);

	// Pointer to the current player's skill set
	TArray<int32>* SkillSetPtr = nullptr;
	
	// Boolean to track if the selection should be locked
	bool* bSelectionLockPtr = nullptr;

	switch (PlayerIndex) {
	case 0:
		SkillSetPtr = &SkillSetP1;
		bSelectionLockPtr = &bSelectionLockP1;
		break;
	case 1:
		SkillSetPtr = &SkillSetP2;
		bSelectionLockPtr = &bSelectionLockP2;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid PlayerIndex: %d"), PlayerIndex);
		return;
	}

	if (SkillSetPtr && bSelectionLockPtr) {
		if (SkillSetPtr->Num() >= 4) {
			SkillSetPtr->Empty();
			*bSelectionLockPtr = false; // Reset selection lock when starting over
		}
		SkillSetPtr->Add(SkillIndex);

		if (SkillSetPtr->Num() == 4) {
			*bSelectionLockPtr = true; // Lock the selection when 4 skills are added
		}

		UE_LOG(LogTemp, Log, TEXT("Selection Lock: %s"), *bSelectionLockPtr ? TEXT("True") : TEXT("False"));
	}

	
}

bool UGameManager::GetSelectionLock(const int PlayerIndex) {
	return PlayerIndex == 0 ? bSelectionLockP1 : bSelectionLockP2;
}


