// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHud.h"
#include "ElfWarsProjectsCharacter.h"
#include "Components/TextBlock.h"

void UPlayerHud::SetUp(AElfWarsProjectsCharacter* Player) {
	Player->OnHealthModification.AddDynamic(this, &UPlayerHud::OnHealthMod);
}

void UPlayerHud::OnHealthMod(float Health) {
	HealthText->SetText(FText::AsNumber(Health));
}

