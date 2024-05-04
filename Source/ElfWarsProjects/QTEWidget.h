// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "QTEWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELFWARSPROJECTS_API UQTEWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UImage* CrossButton{nullptr};

	UPROPERTY(meta=(BindWidget))
	UImage* CircleButton{nullptr};

	UPROPERTY(meta=(BindWidget))
	UImage* SquareButton{nullptr};

	UPROPERTY(meta=(BindWidget))
	UImage* TriangleButton{nullptr};
	
	void ShowRandomButton();
	bool IsButtonVisible(UImage* Button) const;
	
	void HideAllButtons();
	
};
