// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotPopup.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UInventorySlotPopup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite);
		class UInventorySlot* myInventorySlot;

public:
	UFUNCTION(BlueprintCallable)
		void InitializePopup(class UInventorySlot* InventorySlot);

	UFUNCTION(BlueprintCallable)
		void EquipOrUseItem();

	UFUNCTION(BlueprintCallable)
		void ThrowingItem();

	class ATPSPlayer* GetPlayer(); 
};
