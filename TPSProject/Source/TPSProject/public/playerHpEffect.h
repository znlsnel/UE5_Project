// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "playerHpEffect.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UplayerHpEffect : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OpenWidget(bool isHeal);
	


};
