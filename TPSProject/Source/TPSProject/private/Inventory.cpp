// Fill out your copyright notice in the Description page of Project Settings.



#include "Inventory.h"
#include "InventorySlot.h"
#include "InventorySlotPopup.h"
#include "Item.h"
#include "Weapon.h"

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

	if (isEquipable)
	{
		AWeapon* weapon = Cast<AWeapon>(Item);
		weapon->SynchronizeWhitPlayer(myPlayer);
	}
	else
	{
		UInventorySlot* tempSlot = FindFirstEmptySlot();
		if (tempSlot == nullptr) return false;
		tempSlot->Item = Item;
		tempSlot->itemType = Item->itemType;
		tempSlot->ItemIcon = Item->ItemIcon;
		tempSlot->isInUse = true;
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

UInventorySlot* UInventory::FindFirstEmptySlot()
{
	for (auto slot : InventorySlotArray)
	{
		if (slot->isInUse == false) return slot;
	}
	return nullptr;
}
