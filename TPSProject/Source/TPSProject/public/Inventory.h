// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public: // 함수
	UInventory(const FObjectInitializer &ObjectInitializer);
	UFUNCTION(BlueprintCallable)
		void SyncInventorySlot(TArray<class UWidget*> arry);

	void TestFunction();
	class UImage* testImage;

public: // 변수
	class ATPSPlayer* myPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Inventory)
	TArray<class UInventorySlot*> InventorySlotArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Inventory)
	TArray<class UInventorySlot*> equipmentSlot;

	int InventorySize = 10;
};
