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
		ItemIcon = nullptr;
		isInUse = false;
		UpdateInventory();
	}

}
