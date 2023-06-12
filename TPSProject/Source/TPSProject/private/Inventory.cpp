// Fill out your copyright notice in the Description page of Project Settings.



#include "Inventory.h"
#include "InventorySlot.h"
#include "InventorySlotPopup.h"
#include "Item.h"
#include "Weapon.h"
#include "AmmoBox.h"
#include "BuildableItem.h"

#include <Styling/SlateBrush.h>
#include <Engine/Texture2D.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/Image.h>
#include <Net/UnrealNetwork.h>

UInventory::UInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UInventory::SyncInventorySlot(TArray< UWidget*> arry)
{

	for (auto i : arry)
	{
		UInventorySlot* tempSlot = Cast<UInventorySlot>(i);
		InventorySlotArray.Add(tempSlot);
		tempSlot->Inventory = this;
	}
	UpdateInventory();
}



void UInventory::UpdateInventory()
{
	for (auto i : InventorySlotArray)
	{
		i->UpdateInventory();
	}

}

void UInventory::Initialization(ATPSPlayer* player)
{
	myPlayer = player;
}

bool UInventory::AddItemToInventory(AItem* Item)
{

	bool isEquipable = false;
	if (Item->itemType == ItemType::Weapon)
	{
		AWeapon* weapon = Cast<AWeapon>(Item);
		if (myPlayer->playerFire->GetWeapon(weapon->weaponType) == nullptr)
			isEquipable = true;

	}


	if (isEquipable)
	{
		AWeapon* weapon = Cast<AWeapon>(Item);
		myPlayer->playerFire->SetWeapon(weapon);
	}
	else
	{
		UInventorySlot* tempSlot = FindFirstEmptySlot(Item);
		Item->SetActorHiddenInGame(true);
		Item->SetActorEnableCollision(false);

		if (tempSlot == nullptr) {	
			return false;
		}

		tempSlot->Items.Push(Item);
		tempSlot->itemType = Item->itemType;
		tempSlot->ItemIcon = Item->ItemIcon;
		tempSlot->isInUse = true;
		tempSlot->itemCount++;
		tempSlot->UpdateInventory();
	}
	return true;

}


void UInventory::DisablePopup()
{
	if (InventorySlotPopup && InventorySlotPopup->IsInViewport())
	{
		InventorySlotPopup->RemoveFromParent();
	}
}

UInventorySlot* UInventory::FindFirstEmptySlot(AItem* item)
{
	UInventorySlot* result = FindSameItemSlot(item);

	if (IsValid(result))
		return result;

	for (auto slot : InventorySlotArray)
	{
		if (slot->isInUse == false) return slot;
	}

	return nullptr;
}

UInventorySlot* UInventory::FindSameItemSlot(AItem* item)
{
	bool isFindedMatch = false;
	for (auto slot : InventorySlotArray)
	{
		if (slot->itemType == item->itemType)
		{
			if (item->itemType == ItemType::Weapon && Cast<AWeapon>(slot->Items[0])->weaponType
				== Cast<AWeapon>(item)->weaponType) isFindedMatch = true;

			else if (item->itemType == ItemType::Ammo && Cast<AAmmoBox>(slot->Items[0])->ammoType
				== Cast<AAmmoBox>(item)->ammoType) isFindedMatch = true;

			else if (item->itemType == ItemType::Building && Cast<ABuildableItem>(slot->Items[0])->buildableItemType
				== Cast<ABuildableItem>(item)->buildableItemType)
				isFindedMatch = true;

		}
		if (isFindedMatch)
		{
			return slot;
		}
	}
	return nullptr;
}

