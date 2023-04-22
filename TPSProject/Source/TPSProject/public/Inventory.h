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
	
	void UpdateInventory();
	void Initialization(class ATPSPlayer* player);
	void AddItemToInventory(class AItem* Item);
	void DestructPopup();

public: // 변수
	class UTexture2D* testTexture;
	UPROPERTY(BlueprintReadOnly)
		class ATPSPlayer* myPlayer;
	int32 top = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Inventory)
	TArray<class UInventorySlot*> InventorySlotArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Inventory)
	TArray<class UInventorySlot*> equipmentSlot;
	class UInventorySlotPopup* InventorySlotPopup;

	int InventorySize = 10;
};
