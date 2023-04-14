// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include "ScreenUI.h"


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
