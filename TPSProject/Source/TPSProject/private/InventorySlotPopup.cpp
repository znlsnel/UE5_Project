// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotPopup.h"
#include "InventorySlot.h"
#include "Inventory.h"
#include "Weapon.h"
#include "AmmoBox.h"
#include "TPSPlayer.h"
#include "BuildableItem.h"

#include <Blueprint/WidgetLayoutLibrary.h>


void UInventorySlotPopup::InitializePopup(UInventorySlot* InventorySlot)
{
	
	if (IsInViewport()) return;

	myInventorySlot = InventorySlot;
	myInventorySlot->Inventory->DisablePopup();


	if (myInventorySlot->Items.IsEmpty()) return;
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
		AWeapon* weapon = Cast<AWeapon>(myInventorySlot->Items.Last());
		myInventorySlot->RemoveItemFromInventory();

		//weapon->SynchronizeWhitPlayer(myInventorySlot->Inventory->myPlayer);
		weapon->SynchronizeWhitPlayer(GetPlayer());
	}
		break;
	case ItemType::Ammo:
	{
		AAmmoBox* ammoBox = Cast<AAmmoBox>(myInventorySlot->Items.Last());
		bool isAmmoBoxUsed = ammoBox->UseAmmoBox();
		if (isAmmoBoxUsed) myInventorySlot->RemoveItemFromInventory();
	}
		break;

	case ItemType::Building:
	{
		ABuildableItem* item = Cast<ABuildableItem>(myInventorySlot->Items.Last());

		if (item->isBuild == false)
		{
			item->UseItem(myInventorySlot);
		}
		else
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("IsBuild"));
	}
	}
	RemoveFromParent();
}

void UInventorySlotPopup::ThrowingItem()
{
	AItem* item = myInventorySlot->Items.Last();

	item->DropItemOnGround();

	myInventorySlot->RemoveItemFromInventory();
	RemoveFromParent();
}

ATPSPlayer* UInventorySlotPopup::GetPlayer()
{
	return myInventorySlot->Inventory->myPlayer; 
}
