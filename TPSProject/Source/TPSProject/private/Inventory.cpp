// Fill out your copyright notice in the Description page of Project Settings.



#include "Inventory.h"
#include "InventorySlot.h"
#include "InventorySlotPopup.h"
#include "Item.h"

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

void UInventory::AddItemToInventory(AItem* Item)
{
	UInventorySlot* tempSlot = InventorySlotArray[top++];
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("AddITem! Index : %d"), top - 1));

	tempSlot->Item = Item;
	tempSlot->itemType = Item->itemType;
	tempSlot->ItemIcon = Item->ItemIcon;
	tempSlot->UpdateInventory();
}

void UInventory::DestructPopup()
{
	if (InventorySlotPopup)
	{
		InventorySlotPopup->RemoveFromParent();
		InventorySlotPopup->Destruct();
	}
}
