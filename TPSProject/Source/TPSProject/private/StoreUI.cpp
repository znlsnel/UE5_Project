// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreUI.h"
#include "TPSPlayer.h"
#include "ScreenUI.h"
#include "PlayerUI.h"

void UStoreUI::UpgradePlayerAttack()
{
	if (Money < (AdditionalPower * 100) + 10) return;
	Money -= (AdditionalPower * 100) + 10;

	AdditionalPower += 0.1f;
	myPlayer->AdditionalAttackPower += AdditionalPower;
}

void UStoreUI::UpgradePlayerHP()
{
	if (Money < (AdditionalHp * 100) + 10) return;
	Money -= (AdditionalHp * 100) + 10;

	AdditionalHp += 0.1f;
	myPlayer->hp += myPlayer->hp * AdditionalHp;
	myPlayer->playerUI->screenUI->UpdateScreenUI();
}
