// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildableItemCheckUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UBuildableItemCheckUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OpenCheckUI();
public:
	UPROPERTY(BlueprintReadWrite)
		class ABuildableItem* parentItem;
};
