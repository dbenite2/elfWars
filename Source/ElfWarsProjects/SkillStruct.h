// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillStruct.generated.h"

USTRUCT(BlueprintType)

struct ELFWARSPROJECTS_API FSkillStruct
{	

	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	float Weight;

	FSkillStruct() : Weight(0.0f) {}

	FSkillStruct(FName InName, float InWeight) : Name(InName), Weight(InWeight) {}	
};
