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

		//ConstructorHelpers::FObjectFinder<UTexture2D> tempTexture(TEXT("Texture2D'/Game/Megascans/Surfaces/Old_Fabric_sbdkmxp0/T_Old_Fabric_sbdkmxp0_2K_D.T_Old_Fabric_sbdkmxp0_2K_D"));

		//if (tempTexture.Succeeded()){
		//	ItemIcon = tempTexture.Object;
		//}
		//else
		//ItemIcon = nullptr;
		ItemIcon = defaultIcon;
		isInUse = false;
		UpdateInventory();
	}

}
