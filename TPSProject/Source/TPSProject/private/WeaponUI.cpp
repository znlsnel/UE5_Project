// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUI.h"
#include "TPSPlayer.h"
#include "PlayerFire.h"
#include "PlayerMove.h"

void UWeaponUI::ChangeWeapon(WeaponType weaponType)
{

	if (IsValid(player) == false) return;
	player->SetPlayerMouse(false);

	player->playerFire->EquipWeapon(weaponType);

}

WeaponType UWeaponUI::FindSelectWeaponType()
{
	if (IsValid(player) == false) return WeaponType::None;

	UPlayerMove* pMove = Cast<UPlayerMove>(player->playerMove);

	float vertical = pMove->turnValue;
	float horizontal = pMove->lookUpValue;

	if (vertical > 2)
	{
		if (horizontal < -2)
			return WeaponType::Rifle;
		else if (horizontal > 2)
			return WeaponType::Bow;
	}
	else if (vertical < -2)
	{
		if (horizontal < -2)
			return WeaponType::Shotgun;
		else if (horizontal > 2)
			return WeaponType::Pistol;
	}

	return WeaponType::Sword;

}
