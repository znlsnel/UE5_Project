// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemStoreUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UItemStoreUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void GetMouseInput();

	UFUNCTION(BlueprintCallable)
		void ClosePopup();

public:
	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* ItemInListPopup;
};
