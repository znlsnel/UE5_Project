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
	
public: // ÇÔ¼ö
	UFUNCTION(BlueprintImplementableEvent)
		void testChangeImage();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		FItemInfoinInventory ItemInfo;

};
