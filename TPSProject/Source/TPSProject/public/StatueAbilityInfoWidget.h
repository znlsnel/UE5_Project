// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StatueAbilityWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatueAbilityInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UStatueAbilityInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitWidget(FStatueAbility* ability);


	void OpenWidget();

	void CloseWidget();

public:
	UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OnAnim;


	UPROPERTY(BlueprintReadOnly)
		FString AbilityName = "";

	UPROPERTY(BlueprintReadOnly)
		FString AbilityInfo = "";

	UPROPERTY(BlueprintReadOnly)
		FString Value = "";

	UPROPERTY(BlueprintReadOnly)
		FString Cost = "";

};
