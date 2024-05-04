// Fill out your copyright notice in the Description page of Project Settings.


#include "QTEWidget.h"

#include "Kismet/KismetMathLibrary.h"

void UQTEWidget::ShowRandomButton() {
	HideAllButtons();

	int32 ButtonIndex = UKismetMathLibrary::RandomIntegerInRange(0,3);
	switch(ButtonIndex) {
	case 0:
		CrossButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case 1:
		CircleButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2:
		SquareButton->SetVisibility(ESlateVisibility::Visible);
		break;
	case 3:
		TriangleButton->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}

void UQTEWidget::HideAllButtons() {
	if (CrossButton) CrossButton->SetVisibility(ESlateVisibility::Hidden);
	if (CircleButton) CircleButton->SetVisibility(ESlateVisibility::Hidden);
	if (SquareButton) SquareButton->SetVisibility(ESlateVisibility::Hidden);
	if (TriangleButton) TriangleButton->SetVisibility(ESlateVisibility::Hidden);
}

bool UQTEWidget::IsButtonVisible(UImage* Button) const {
	return Button && Button->GetVisibility() == ESlateVisibility::Visible;
}



