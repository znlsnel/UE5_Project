// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Item.h"
#include <Containers/Array.h>

void UInventorySlot::RemoveItemFromInventory()
{
	//Items.Last()->SetActorHiddenInGame(false);
	Items.Pop();

	if (--itemCount == false)
	{
		itemType = ItemType::None;

		ConstructorHelpers::FObjectFinder<UTexture2D> tempT(TEXT("Texture2D'/Game/Megascans/Surfaces/Old_Fabric_sbdkmxp0/T_Old_Fabric_sbdkmxp0_2K_D.T_Old_Fabric_sbdkmxp0_2K_D'"));

		if (tempT.Succeeded()){
			ItemIcon = tempT.Object;
		}
		else
		ItemIcon = nullptr;
		isInUse = false;
		UpdateInventory();
	}

}
