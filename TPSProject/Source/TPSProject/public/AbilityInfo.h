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

	
	void HoverWidget();
	void UnHoverWidget();

public:
	UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OnAnim;

	UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OffAnim;


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
