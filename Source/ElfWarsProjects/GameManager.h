// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SkillStruct.h"
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
	void SetSkillSet(const int PlayerIndex, const FSkillStruct skill);
	
	UFUNCTION(CallInEditor, BlueprintCallable)
	TArray<FSkillStruct> GetSkillSet(const int& PlayerIndex);
	
	UFUNCTION(CallInEditor, BlueprintCallable)
	bool GetSelectionLock(const int PlayerIndex);

	UFUNCTION(CallInEditor, BlueprintCallable)
	bool AllPlayersLockedIn() const;

	// get character references and pass them to the asking player.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Players")
	TArray<ACharacter*> AvailablePlayers;
	
	void RegisterPlayer(ACharacter* Character);

	ACharacter* GetPlayer(int32 PlayerIndex);

	void CleanUpRegisteredPlayers();

	void EndRound(const int& PlayerIndex);

private:
	UPROPERTY(EditAnywhere, Blueprintable, Category="Skills")
	TArray<FSkillStruct> SkillSetP1 {{"Kick", 5.f}, {"Hit", 2.5f}, {"Magic_01", 7.5f}, {"Magic_02", 3.f}};
	
	UPROPERTY(EditAnywhere, Blueprintable, Category="Skills")
	TArray<FSkillStruct> SkillSetP2 {{"Kick", 5.f}, {"Hit", 2.5f}, {"Magic_01", 7.5f}, {"Magic_02", 3.f}};
	
	UPROPERTY(EditAnywhere, Blueprintable, Category="Selection complete")
	bool bSelectionLockP1;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Selection complete")
	bool bSelectionLockP2;

	UPROPERTY()
	int GameRounds{0};

	void CleanSkillSets();
	
};
