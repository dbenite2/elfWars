// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElfWarsProjectsGameMode.h"
#include "ElfWarsProjectsCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameManager.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
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
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());
	// Assuming you have a PlayerStart class that positions players
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	FString PlayerCreationError;
	

	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		// Create a local player and set its controller
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APlayerController* NewPlayerController = GetWorld()->SpawnActor<APlayerController>(APlayerController::StaticClass(), PlayerStarts[i]->GetTransform(), SpawnParams);
		ULocalPlayer* LocalPlayer = GM ? GM->CreateLocalPlayer(i, PlayerCreationError, true) : nullptr;

		if (NewPlayerController && LocalPlayer)
		{
			NewPlayerController->Player = LocalPlayer;

			// Set up the enhanced input subsystem for this player
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (InputSubsystem)
			{
				UInputMappingContext* InputContext = NewObject<UInputMappingContext>(InputSubsystem); // Make sure to create this asset in UE editor
				InputSubsystem->AddMappingContext(InputContext, 1); // Assuming '1' is the priority
			}

			// Spawn the character for this player
			ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->SpawnActor<AElfWarsProjectsCharacter>(AElfWarsProjectsCharacter::StaticClass(), PlayerStarts[i]->GetTransform(), SpawnParams));
			if (PlayerCharacter)
			{
				NewPlayerController->Possess(PlayerCharacter);
				// GM->RegisterPlayer(PlayerCharacter);
			}
		}
	}
	
}

void AElfWarsProjectsGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());
	
	APawn* ControlledPawn = NewPlayer->GetPawn();
	ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
	if (GM && ControlledCharacter) {
		GM->CleanUpRegisteredPlayers();
		GM->RegisterPlayer(ControlledCharacter);
	}
}

