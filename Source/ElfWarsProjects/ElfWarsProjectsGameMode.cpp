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
		// DefaultPawnClass = nullptr;
	}
}

void AElfWarsProjectsGameMode::StartPlay() {
	Super::StartPlay();
}

void AElfWarsProjectsGameMode::BeginPlay() {
	Super::BeginPlay();
	FString ErrorMessage;
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	for (int32 i = 0; i < PlayerStarts.Num(); i++) {
		// Create a local player and set its controller
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		if (ULocalPlayer* LocalPlayer = GM ? GM->CreateLocalPlayer(i, ErrorMessage, true) : nullptr) {
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, i)) {
				PC->Player = LocalPlayer;
			
				// Set up the enhanced input subsystem for this player
				if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					UInputMappingContext* InputContext = NewObject<UInputMappingContext>(InputSubsystem);
					InputSubsystem->AddMappingContext(InputContext, 1); // Assuming '1' is the priority
				}
			}
		}
	}

	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (CurrentLevelName == FName("SkillSelectionLevel")) {
		
		if(UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport()) {
			ViewportClient->SetForceDisableSplitscreen(false);
		}
	}
	
	if (CurrentLevelName != FName("SkillSelectionLevel")) {
		if(UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport()) {
			ViewportClient->SetForceDisableSplitscreen(true);
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


