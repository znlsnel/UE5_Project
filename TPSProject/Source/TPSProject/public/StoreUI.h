// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoreUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UStoreUI : public UUserWidget
{
	GENERATED_BODY()

public:
	class ATPSPlayer* myPlayer;

	UPROPERTY(BlueprintReadOnly)
		int Money = 100;

	UPROPERTY(BlueprintReadOnly)
		float AdditionalPower = 0.f;

	UPROPERTY(BlueprintReadOnly)
		float AdditionalHp = 0.f;

public:
	UFUNCTION(BlueprintCallable)
		void UpgradePlayerAttack();

	UFUNCTION(BlueprintCallable)
		void UpgradePlayerHP();

};