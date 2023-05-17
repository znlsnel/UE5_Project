// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreUI.h"
#include "TPSPlayer.h"
#include "ScreenUI.h"
#include "PlayerUI.h"
#include "StoreActor.h"
#include <Kismet/KismetSystemLibrary.h>

void UStoreUI::UpgradePlayerAttack()
{
	//storeActor->UpgradeAttackInServer();
	UpgradeAttack();
}

void UStoreUI::UpgradeAttack()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Upgrade Attack!"));
	if (Money < AttackCost) return;
	Money -= AttackCost;
	AttackCost += 10;
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Money : %d"), Money));
	AdditionalPower += 0.1f;

	if (storeActor == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No StoreActor;;"));
	}
	else if (GetPlayer() == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NoPlayer;;"));
	}
	else
		GetPlayer()->UpdateAttackAndHp(true, AdditionalPower);
}



void UStoreUI::UpgradePlayerHP()
{

	//storeActor->UpgradeHpInServer();
	UpgradeHp();
}

void UStoreUI::UpgradeHp()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Upgrade HP!"));
	if (Money < HpCost) return;
	Money -= HpCost;
	HpCost += 10.f;

	AdditionalHp += 0.1f;

	if (storeActor == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No StoreActor;;"));
	}
	else if (GetPlayer() == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NoPlayer;;"));
	}
	else
	{
		GetPlayer()->UpdateAttackAndHp(false, AdditionalHp);

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
