// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public: // 함수
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateInventory();
	void RemoveItemFromInventory();

public:// 변수
	bool isInUse = false;
	
	UPROPERTY(BlueprintReadOnly)
		class UInventory* Inventory;

	UPROPERTY(BlueprintReadOnly)
		ItemType itemType = ItemType::None;

	UPROPERTY(BlueprintReadWrite) 
		class UTexture2D* ItemIcon;
	
	UPROPERTY(EditAnywhere)
		class UTexture2D* defaultIcon;

	UPROPERTY()
		TArray<class AItem*> Items;

	UPROPERTY(BlueprintReadWrite)
		int itemCount = 0;
};
