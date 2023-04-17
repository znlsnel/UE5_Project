// Fill out your copyright notice in the Description page of Project Settings.
#include "ScreenUI.h"
#include "PlayerFire.h"
#include "TPSPlayer.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Components/Image.h>

void UScreenUI::UpdateScreenUI()
{
	hpRatio = static_cast<float>(myPlayer->hp) / static_cast<float>(myPlayer->initialHp);
	hpInfo = FString::Printf(TEXT("%d / %d"), myPlayer->hp, myPlayer->initialHp);
}

void UScreenUI::Initialization(ATPSPlayer* player)
{
	AddToViewport();
	myPlayer = player;
	myPlayer->tickUpdateFunctions.AddUObject(this, &UScreenUI::UpdateScreenUI);
	WeaponSwap();
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

	
	UKismetSystemLibrary::PrintString(GetWorld(), tempStr);
}
