// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityInfo.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UAbilityInfo : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitWidget(FString SkillName, FString SkillInfomation, int currValue, int nextValue);

	UFUNCTION(BlueprintImplementableEvent)
		void HoverWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void UnHoverWidget();


public:
	UPROPERTY(BlueprintReadOnly)
		FString SkillName = "";

	UPROPERTY(BlueprintReadOnly)
		FString SkillInfomation = "";

	UPROPERTY(BlueprintReadOnly)
		FString CurrValue = "";

	UPROPERTY(BlueprintReadOnly)
		FString NextValue = "";
};
