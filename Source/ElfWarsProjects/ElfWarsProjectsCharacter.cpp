// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElfWarsProjectsCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameManager.h"
#include "QTEWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AElfWarsProjectsCharacter

AElfWarsProjectsCharacter::AElfWarsProjectsCharacter()
{
	

	
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AElfWarsProjectsCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}


void AElfWarsProjectsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Get the current level of the character
	CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	// Check if the current level name matches the target level name
	if(CurrentLevelName != FName(TEXT("SkillSelectionLevel"))) {

		if (!PlayerHudTemplate) return;

		// Get the gameManager info
		GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(this));

		//Add Input Mapping Context
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		// get the skills from the GameManager

		if (PlayerController && GameManager)
		{
			const int PlayerIndex = PlayerController->GetLocalPlayer()->GetLocalPlayerIndex();
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
			OtherPlayer = Cast<AElfWarsProjectsCharacter>(GameManager->GetPlayer(PlayerIndex == 0 ? 1 : 0));
			AvailableSkills = GameManager->GetSkillSet(PlayerIndex);
			PlayerHud = CreateWidget<UUserWidget>(PlayerController, PlayerHudTemplate);
			PlayerHud->AddToPlayerScreen();
			QTEWidget = GetQTEWidget(PlayerHud);
		}
		UE_LOG(LogTemp, Warning, TEXT("Character is in the target level."));
		PlayerHealth = MaxPlayerHealth;
		StartQTE();
	} 
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AElfWarsProjectsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AElfWarsProjectsCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AElfWarsProjectsCharacter::Look);


		// Skills
		EnhancedInputComponent->BindAction(ExecuteCrossButtonSkill, ETriggerEvent::Triggered, this, &AElfWarsProjectsCharacter::CrossButtonSkill);

		EnhancedInputComponent->BindAction(ExecuteCircleButtonSkill, ETriggerEvent::Triggered, this, &AElfWarsProjectsCharacter::CircleButtonSkill);

		EnhancedInputComponent->BindAction(ExecuteSquareButtonSkill, ETriggerEvent::Triggered, this, &AElfWarsProjectsCharacter::SquareButtonSkill);

		EnhancedInputComponent->BindAction(ExecuteTriangleButtonSkill, ETriggerEvent::Triggered, this, &AElfWarsProjectsCharacter::TriangleButtonSkill);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AElfWarsProjectsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AElfWarsProjectsCharacter::Look(const FInputActionValue& Value)
{
	// // input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AElfWarsProjectsCharacter::CrossButtonSkill() {
	FSkillStruct SelectedSkill = AvailableSkills[0];
	if (QTEWidget && QTEWidget->IsButtonVisible(QTEWidget->CrossButton)) {
		Hit(SelectedSkill);
		UE_LOG(LogTemp, Log, TEXT("Doing skill in slot 1: %f"), SelectedSkill.Weight)
	}
}

void AElfWarsProjectsCharacter::SquareButtonSkill() {
	FSkillStruct SelectedSkill = AvailableSkills[1];
	if (QTEWidget && QTEWidget->IsButtonVisible(QTEWidget->SquareButton)) {
		Hit(SelectedSkill);
		UE_LOG(LogTemp, Log, TEXT("Doing skill in slot 2: %f"), SelectedSkill.Weight)
	}
}

void AElfWarsProjectsCharacter::TriangleButtonSkill() {
	FSkillStruct SelectedSkill = AvailableSkills[2];
	if (QTEWidget && QTEWidget->IsButtonVisible(QTEWidget->TriangleButton)) {
		Hit(SelectedSkill);
		UE_LOG(LogTemp, Log, TEXT("Doing skill in slot 3: %f"), SelectedSkill.Weight)
	}
}

void AElfWarsProjectsCharacter::CircleButtonSkill() {
	FSkillStruct SelectedSkill = AvailableSkills[3];
	if (QTEWidget && QTEWidget->IsButtonVisible(QTEWidget->CircleButton)) {
		Hit(SelectedSkill);
		UE_LOG(LogTemp, Log, TEXT("Doing skill in slot 4: %f"), SelectedSkill.Weight)
	}
}

void AElfWarsProjectsCharacter::Hit(FSkillStruct& Skill) {
	// if same skill type add 5 to skill damage
	// Skill.Weight *= 2;
	if (OtherPlayer) {
		OtherPlayer->ReceiveDamage(Skill);
	}
	QTERepeatInterval -= .02f;
	EndQTE();
}

void AElfWarsProjectsCharacter::ReceiveDamage(FSkillStruct& Skill) {
	// if weak to skillType + 10 skill damage
	// is same type skill damage / 2
	float DamageReceived = 0.f;
	if (Type == "Dark") {
		DamageReceived = Skill.Weight * 2.0f;
	}
	PlayerHealth -= Skill.Weight;
	PlayerHealth = FMath::Clamp(PlayerHealth, 0, MaxPlayerHealth);
	
	OnHealthModification.Broadcast(PlayerHealth);

	if (PlayerHealth <= 0) {
		CancelQTE();
		// switch to skill selection or end game ( should ask game manager for this )
		if(GameManager) {
			GameManager->EndRound(0);
		}
	}
}

float AElfWarsProjectsCharacter::GetHealth() const {
	return PlayerHealth;
}

float AElfWarsProjectsCharacter::GetHealthPercentage() const {
	return PlayerHealth / MaxPlayerHealth;
}

UQTEWidget* AElfWarsProjectsCharacter::GetQTEWidget(UUserWidget* ParentWidget) {
	if (!ParentWidget) return nullptr;
	return Cast<UQTEWidget>(ParentWidget->WidgetTree->FindWidget("WBP_QTE"));
}

void AElfWarsProjectsCharacter::StartQTE() {
	QTEWidget->ShowRandomButton();
	// float QTEDuration = 1.5f; // TODO: Change this value to be based on the character type instead of fixed
	// float QTERepeatInterval = 3.5f;
	GetWorldTimerManager().SetTimer(QTETimerHandle, this, &AElfWarsProjectsCharacter::EndQTE, QTEDuration, false);
	GetWorldTimerManager().SetTimer(QTERepeatingHandle, this, &AElfWarsProjectsCharacter::StartQTE, QTERepeatInterval, false);
}

void AElfWarsProjectsCharacter::EndQTE() const {
	QTEWidget->HideAllButtons();
}

void AElfWarsProjectsCharacter::CancelQTE() {
	GetWorldTimerManager().ClearTimer(QTETimerHandle);
	GetWorldTimerManager().ClearTimer(QTERepeatingHandle);
	QTEWidget->HideAllButtons();
}

