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
public: // �Լ�
	UInventory(const FObjectInitializer &ObjectInitializer);
	UFUNCTION(BlueprintCallable)
		void SyncInventorySlot(TArray<class UWidget*> arry);
	
	void UpdateInventory();
	void Initialization(class ATPSPlayer* player);

	bool AddItemToInventory(class AItem* Item);

	void DisablePopup();
	class UInventorySlot* FindFirstEmptySlot(class AItem* item);
	class UInventorySlot* FindSameItemSlot(class AItem* item);
	void GetInventorySlot(TArray<int>& ItemIDArr);
	void SetInventorySlot(TArray<int>& ItemIDArr);

public: // ����
	class UTexture2D* testTexture;
	UPROPERTY(BlueprintReadOnly)
		class ATPSPlayer* myPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Inventory)
	TArray<class UInventorySlot*> InventorySlotArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Inventory)
	TArray<class UInventorySlot*> equipmentSlot;
	class UInventorySlotPopup* InventorySlotPopup;

	int InventorySize = 10;
};
