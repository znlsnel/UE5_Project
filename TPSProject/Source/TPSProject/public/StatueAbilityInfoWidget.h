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


	UFUNCTION(BlueprintImplementableEvent)
		void OpenWidget();

	UFUNCTION(BlueprintImplementableEvent)
		void CloseWidget();

public:
	UPROPERTY(BlueprintReadOnly)
		FString AbilityName = "";

	UPROPERTY(BlueprintReadOnly)
		FString AbilityInfo = "";

	UPROPERTY(BlueprintReadOnly)
		FString Value = "";

	UPROPERTY(BlueprintReadOnly)
		FString Cost = "";

};
