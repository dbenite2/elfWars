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

void AElfWarsProjectsGameMode::BeginPlay() {
	Super::BeginPlay();
	FString ErrorMessage;
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		int32 NewPlayerIndex = GameInstance->GetNumLocalPlayers(); // Get next player index
		ULocalPlayer* NewLocalPlayer = GameInstance->CreateLocalPlayer(NewPlayerIndex, ErrorMessage, true);
		if (NewLocalPlayer)
		{
			APlayerController* NewPlayerController = Cast<APlayerController>(NewLocalPlayer->GetPlayerController(GetWorld()));
			if (NewPlayerController)
			{
				// Optionally spawn a pawn for this player controller and possess it
				APawn* NewPawn = SpawnDefaultPawnFor(NewPlayerController, FindPlayerStart(NewPlayerController));
				if (NewPawn)
				{
					NewPlayerController->Possess(NewPawn);
				}
			}
		}
	}
}
