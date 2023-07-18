// Fill out your copyright notice in the Description page of Project Settings.
#include "ScreenUI.h"
#include "PlayerFire.h"
#include "TPSPlayer.h"
#include "Inventory.h"
#include "Weapon.h"
#include "PlayerAbilityComp.h"
#include "TPSProjectGameModeBase.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/Image.h>

#include <GameFramework/PlayerState.h>



void UScreenUI::UpdateScreenUI()
{
	hpRatio = static_cast<float>(myPlayer->hp) / static_cast<float>(myPlayer->maxHp);
	hpInfo = FString::Printf(TEXT("%d / %d"), myPlayer->hp, myPlayer->maxHp);
}

void UScreenUI::ATVWidget()
{
	if (IsInViewport()) return;

	StartWidget();
	WeaponSwap();
}

void UScreenUI::RespawnTimeLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(respawnTimer);
	if (RespawnTime <= 0) {
		RespawnEvent(false);
		return;
	}
	RespawnTime--;
	RespawnTimeUpdate();

	GetWorld()->GetTimerManager().SetTimer(respawnTimer, this, &UScreenUI::RespawnTimeLoop, 1.f, false);
}

void UScreenUI::Initialization(ATPSPlayer* player)
{
	if (IsValid(this) == false) return;
	myPlayer = player;

	inventory = CreateWidget<UInventory>(GetWorld());
	inventory->Initialization(myPlayer);
	myAbilityComp = myPlayer->abilityComp;
	myAbilityComp->myScreenUI = this;
	myGameMode = Cast<ATPSProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}



void UScreenUI::ToggleInventory()
{
	if (bOpenInventory)
	{

		bOpenInventory = false;
	}
	else
	{

		bOpenInventory = true;
	}

	BP_ToggleInventory();
}

void UScreenUI::ToggleInventory(bool On)
{
	if (On == bOpenInventory) 
		return;

	InventoryOnOff(On);
}

void UScreenUI::UseSkillSlot(SkillType type)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("UseSkillSlot"));
	bool result = myAbilityComp->UseSkill(type);
	ToggleSkillSlot(type, result);
}

void UScreenUI::BasicSkillCoolTime(bool dashOrShield, float CoolTime)
{
	if (dashOrShield) {
		DashCoolTime = CoolTime;
		DashTime = 0.f;
	}
	else {
		ShieldCoolTime = CoolTime;
		ShieldTime = 0.f;
	}

	bBasicSkillLoopEnd = true;
	BasicSkillCoolTimeLoop();

	GetWorld()->GetTimerManager().ClearTimer(basicSkillLoopEndTimer);
	GetWorld()->GetTimerManager().SetTimer(basicSkillLoopEndTimer, FTimerDelegate::CreateLambda(
		[&]() {
			bBasicSkillLoopEnd = false;
		}
	), CoolTime + 0.5f, false); 
}

void UScreenUI::BasicSkillCoolTimeLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(basicSkillLoopTimer);
	if (bBasicSkillLoopEnd == false) {
		return;
	}

	if (DashTime < 0.96f) {
		DashTime += 0.05f / DashCoolTime;
	}
	else DashTime = 1.f;

	if (ShieldTime < 0.96f) {
		ShieldTime += 0.05f / ShieldCoolTime;
	}
	else ShieldTime = 1.f;


	GetWorld()->GetTimerManager().SetTimer(basicSkillLoopTimer, this, &UScreenUI::BasicSkillCoolTimeLoop, 0.05f, false);
}


FString UScreenUI::UpdateAmmoCount()
{
	AWeapon* weapon = myPlayer->playerFire->currWeapon;
	if (weapon == nullptr) return FString("");
	FString ammoString = FString::Printf(TEXT("%d / %d"), weapon->currAmmo, weapon->Ammo);
	if (weapon->weaponType == WeaponType::Sword)
		ammoString = FString::Printf(TEXT(""));
	else if (weapon->weaponType == WeaponType::Bow)
		ammoString = FString::Printf(TEXT("%d"), weapon->currAmmo);

	return ammoString;
}



void UScreenUI::WeaponSwap_Implementation()
{
	if (IsValid(myPlayer->playerFire->currWeapon) == false) return;
	switch (myPlayer->playerFire->currWeapon->weaponType)
	{
	case WeaponType::Rifle:
		currWeaponTexture = RifleUI;
		break;
	case WeaponType::Shotgun:
		currWeaponTexture = ShotgunUI;
		break;
	case WeaponType::Pistol:
		currWeaponTexture = PistolUI;
		break;
	case WeaponType::Bow:
		currWeaponTexture = BowUI;
		break;
	case WeaponType::Sword:
		currWeaponTexture = SwordUI;
		break;
	}
}
