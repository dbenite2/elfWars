// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElfWarsProjectsGameMode.h"
#include "ElfWarsProjectsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AElfWarsProjectsGameMode::AElfWarsProjectsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
