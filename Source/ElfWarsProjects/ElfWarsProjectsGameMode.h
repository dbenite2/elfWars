// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ElfWarsProjectsGameMode.generated.h"

UCLASS(minimalapi)
class AElfWarsProjectsGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

public:
	AElfWarsProjectsGameMode();
};



