// Fill out your copyright notice in the Description page of Project Settings.



#include "Inventory.h"
#include "InventorySlot.h"
#include "InventorySlotPopup.h"
#include "Item.h"
#include "Weapon.h"
#include "AmmoBox.h"

#include <Styling/SlateBrush.h>
#include <Engine/Texture2D.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/Image.h>

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

		switch (weapon->weaponSlotType)
		{
		case WeaponSlotType::PrimarySlot:
			if (myPlayer->playerFire->primaryWeapon == nullptr) 
				isEquipable = true;
			break;
		case WeaponSlotType::SecondarySlot:
			if (myPlayer->playerFire->secondaryWeapon == nullptr)
				isEquipable = true;
			break;
		}
	}
	else if (Item->itemType == ItemType::Ammo)
	{

	}


	if (isEquipable)
	{
		AWeapon* weapon = Cast<AWeapon>(Item);
		weapon->SynchronizeWhitPlayer(myPlayer);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("GetItem"));
		UInventorySlot* tempSlot = FindFirstEmptySlot(Item);
		if (tempSlot == nullptr) return false;
		tempSlot->Items.Push(Item);
		tempSlot->itemType = Item->itemType;
		tempSlot->ItemIcon = Item->ItemIcon;
		tempSlot->isInUse = true;
		tempSlot->itemCount++;
		tempSlot->UpdateInventory();
		Item->SetActorHiddenInGame(true);
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
	bool isFindedMatch = false;
	for (auto slot : InventorySlotArray)
	{
		if (slot->itemType == item->itemType)
		{
			if (item->itemType == ItemType::Weapon && Cast<AWeapon>(slot->Items[0])->weaponType
				== Cast<AWeapon>(item)->weaponType) isFindedMatch = true;
			
			else if (item->itemType == ItemType::Ammo && Cast<AAmmoBox>(slot->Items[0])->ammoType
				== Cast<AAmmoBox>(item)->ammoType) isFindedMatch = true;
		}
		if (isFindedMatch)
		{
			return slot;
		}
	}


	for (auto slot : InventorySlotArray)
	{
		if (slot->isInUse == false) return slot;
	}

	return nullptr;
}

