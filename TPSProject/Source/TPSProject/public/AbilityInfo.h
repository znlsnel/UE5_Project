// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerAbilityComp.h"

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
	void InitWidget(FSkillInfo* skillInfo, bool StartHover = true);

	

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

	UPROPERTY(BlueprintReadOnly)
		FString CoolTimeText = "";
};
