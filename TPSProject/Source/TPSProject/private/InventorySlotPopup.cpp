// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotPopup.h"
#include "InventorySlot.h"
#include "Inventory.h"

void UInventorySlotPopup::InitializePopup(UInventorySlot* InventorySlot)
{
	myInventorySlot = InventorySlot;
	myInventorySlot->Inventory->DestructPopup();
	myInventorySlot->Inventory->InventorySlotPopup = this;
}

void UInventorySlotPopup::EquipOrUseItem()
{

}

void UInventorySlotPopup::ThrowingItem()
{

}
