// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ElfWarsProjectsCharacter.generated.h"

class UGameManager;
class UQTEWidget;
struct FSkillStruct;
class UCSkillSelection;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthSignature, float, Health);

UCLASS(config=Game)
class AElfWarsProjectsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExecuteCrossButtonSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExecuteSquareButtonSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExecuteTriangleButtonSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExecuteCircleButtonSkill;

	UPROPERTY()
	AElfWarsProjectsCharacter* OtherPlayer{nullptr};

public:
	AElfWarsProjectsCharacter();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Type")
	FString Type;

	UFUNCTION(CallInEditor, Blueprintable)
	void Hit(FSkillStruct& Skill);

	UFUNCTION(CallInEditor, Blueprintable, BlueprintCallable)
	void ReceiveDamage(FSkillStruct& Skill);

	UPROPERTY(BlueprintAssignable)
	FOnHealthSignature OnHealthModification;

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayerHudTemplate;

	UPROPERTY()
	UUserWidget* PlayerHud{nullptr};

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void CrossButtonSkill();

	void SquareButtonSkill();

	void TriangleButtonSkill();

	void CircleButtonSkill();

	UPROPERTY(EditAnywhere, Blueprintable, Category="Skill Selector")
	TArray<FSkillStruct> AvailableSkills;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Player Health")
	float PlayerHealth{0.f};

	UPROPERTY(EditAnywhere, Blueprintable, Category="Player Health")
	float MaxPlayerHealth{100.f};

	UPROPERTY()
	UQTEWidget* QTEWidget{nullptr};

	UQTEWidget* GetQTEWidget(UUserWidget* ParentWidget);

	void StartQTE();

	void CancelQTE();

	UPROPERTY(EditAnywhere);
	float QTEDuration{1.5f};

	UPROPERTY(EditAnywhere);
	float QTERepeatInterval{2.f};
	
	FTimerHandle QTETimerHandle;

	FTimerHandle QTERepeatingHandle;

	void EndQTE() const;

	FString CurrentLevelName;

	UPROPERTY()
	UGameManager* GameManager{nullptr};

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

