// Fill out your copyright notice in the Description page of Project Settings.



#include "Inventory.h"
#include "InventorySlot.h"
#include "SlateBasics.h"

#include <Styling/SlateBrush.h>
#include <Engine/Texture2D.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/Image.h>

UInventory::UInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//ConstructorHelpers::FObjectFinder<UTexture2D> tempImage(TEXT("Texture2D'/Game/LyraResources/UI/Hud/Art/T_UI_Icon_RangedWeapons_Pistol.T_UI_Icon_RangedWeapons_Pistol'"));


	UTexture2D* tempTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/LyraResources/UI/Hud/Art/T_UI_Icon_RangedWeapons_Pistol.T_UI_Icon_RangedWeapons_Pistol'"));
	
	
	if (tempTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(tempTexture);
		testImage->SetBrush(Brush);
	}


}

void UInventory::SyncInventorySlot(TArray< UWidget*> arry)
{
	for (auto i : arry)
	{
		UInventorySlot* tempSlot = Cast<UInventorySlot>(i);

		InventorySlotArray.Add(tempSlot);
	}
}

void UInventory::TestFunction()
{
	for (auto i : InventorySlotArray)
	{
		UKismetSystemLibrary::PrintString(GetWorld(),TEXT("TestFunction"));
		i->ItemInfo.itemImage = testImage;
		i->testChangeImage();
	}
	
}
