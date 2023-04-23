// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotPopup.h"
#include "InventorySlot.h"
#include "Inventory.h"
#include "Weapon.h"

#include <Blueprint/WidgetLayoutLibrary.h>


void UInventorySlotPopup::InitializePopup(UInventorySlot* InventorySlot)
{
	if (IsInViewport()) return;

	myInventorySlot = InventorySlot;
	myInventorySlot->Inventory->DisablePopup();

	if (myInventorySlot->Item == nullptr) return;
	AddToViewport();
	
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	FVector2D genPos = MousePosition + FVector2D(-100, 30);

	SetPositionInViewport(genPos, false);
	myInventorySlot->Inventory->InventorySlotPopup = this;


}

void UInventorySlotPopup::EquipOrUseItem()
{
	switch (myInventorySlot->itemType)
	{
	case ItemType::Weapon:
	{
		AWeapon* weapon = Cast<AWeapon>(myInventorySlot->Item);
		myInventorySlot->RemoveItemFromInventory();
		weapon->SynchronizeWhitPlayer(myInventorySlot->Inventory->myPlayer);
	}
		break;
	case ItemType::Ammo:
		myInventorySlot->RemoveItemFromInventory();

		break;
	}
	RemoveFromParent();
}

void UInventorySlotPopup::ThrowingItem()
{
	myInventorySlot->Item->DropItemOnGround();
	myInventorySlot->RemoveItemFromInventory();
	RemoveFromParent();

}
