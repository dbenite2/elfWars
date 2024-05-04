// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

class UTextBlock;
class AElfWarsProjectsCharacter;
/**
 * 
 */
UCLASS()
class ELFWARSPROJECTS_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthText{nullptr};

	void SetUp(AElfWarsProjectsCharacter* Player);

	UFUNCTION()
	void OnHealthMod(float Health);
	
};
