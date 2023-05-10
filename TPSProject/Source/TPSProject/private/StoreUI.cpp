// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreUI.h"
#include "TPSPlayer.h"
#include "ScreenUI.h"
#include "PlayerUI.h"
#include "StoreActor.h"
#include <Kismet/KismetSystemLibrary.h>

void UStoreUI::UpgradePlayerAttack()
{
	storeActor->UpgradeAttackInServer();

}

void UStoreUI::UpgradeAttack()
{
	if (Money < (AdditionalPower * 100) + 10) return;
	Money -= ((AdditionalPower / 0.1f) * 10) + 10;
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Money : %d"), Money));
	AdditionalPower += 0.1f;

	if (storeActor == nullptr)
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No StoreActor;;"));
	else if (GetPlayer() == nullptr) UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NoPlayer;;"));
	else
		GetPlayer()->AdditionalAttackPower = -AdditionalPower;
}



void UStoreUI::UpgradePlayerHP()
{

	storeActor->UpgradeHpInServer();

}

void UStoreUI::UpgradeHp()
{
	if (Money < (AdditionalHp * 100) + 10) return;
	Money -= (AdditionalHp * 100) + 10;

	AdditionalHp += 0.1f;

	if (storeActor == nullptr)
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No StoreActor;;"));
	else if (GetPlayer() == nullptr) UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NoPlayer;;"));
	else
	{
		GetPlayer()->hp += 10;
		GetPlayer()->initialHp += 10;
		if (GetScreenUI())
			GetScreenUI()->UpdateScreenUI();
	}
}

ATPSPlayer* UStoreUI::GetPlayer()
{
	return storeActor->myPlayer;
}

UScreenUI* UStoreUI::GetScreenUI()
{
	return GetPlayer()->playerUI->screenUI;
}
