// Fill out your copyright notice in the Description page of Project Settings.
#include "ScreenUI.h"
#include "PlayerFire.h"
#include "TPSPlayer.h"
#include "Inventory.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Components/Image.h>






void UScreenUI::UpdateScreenUI()
{
	hpRatio = static_cast<float>(myPlayer->hp) / static_cast<float>(myPlayer->initialHp);
	hpInfo = FString::Printf(TEXT("%d / %d"), myPlayer->hp, myPlayer->initialHp);
}

void UScreenUI::Initialization(ATPSPlayer* player)
{
	myPlayer = player;

	myPlayer->tickUpdateFunctions.AddUObject(this, &UScreenUI::UpdateScreenUI);
	inventory = CreateWidget<UInventory>(GetWorld());
	inventory->Initialization(myPlayer);

	AddToViewport();
	WeaponSwap();

}

void UScreenUI::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &UScreenUI::ToggleInventory);
}

void UScreenUI::ToggleInventory()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	if (bOpenInventory)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Close Inventory")));
		pc->bShowMouseCursor = false;
		pc->bEnableClickEvents = false;
		pc->bEnableMouseOverEvents = false;
		bOpenInventory = false;
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Open Inventory")));
		pc->bShowMouseCursor = true;
		pc->bEnableClickEvents = true;
		pc->bEnableMouseOverEvents = true;
		bOpenInventory = true;
	}

	BP_ToggleInventory();
}



void UScreenUI::WeaponSwap_Implementation()
{
	FString tempStr;
	switch (myPlayer->playerFire->currSlot)
	{
	case WeaponSlotType::PrimarySlot:
		tempStr = FString("PrimarySlot");
		primaryInfo.ZOrder = 2;
		secondaryInfo.ZOrder = 0;
		break;
	case WeaponSlotType::SecondarySlot:
		tempStr = FString("SecondarySlot");
		primaryInfo.ZOrder = 0;
		secondaryInfo.ZOrder = 2;
		break;
	}
	
	switch (myPlayer->playerFire->currWeapon)
	{
	case WeaponType::Rifle:
		primaryInfo.Textrue = RifleUI;
		break;
	case WeaponType::Shotgun:
		primaryInfo.Textrue = ShotgunUI;
		break;
	}
	
	if (myPlayer->playerFire->primaryWeapon == nullptr)
		primaryInfo.Textrue = TransParentUI;

	UKismetSystemLibrary::PrintString(GetWorld(), tempStr);
}
