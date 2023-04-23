// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Item.h"

void UInventorySlot::RemoveItemFromInventory()
{
	Item->SetActorHiddenInGame(false);
	itemType = ItemType::None;
	ItemIcon = nullptr;
	Item = nullptr;
	isInUse = false;

	UpdateInventory();
}
