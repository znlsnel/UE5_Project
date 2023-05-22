// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUI.h"
#include "TPSPlayer.h"
#include "PlayerFire.h"
#include "PlayerMove.h"

void UWeaponUI::ChangeWeapon(WeaponType weaponType)
{

	if (IsValid(player) == false) return;
	player->SetPlayerMouse(false);
	
	switch (weaponType)
	{
	case WeaponType::Pistol:
		player->playerFire->EquipSecondaryWeapon();
		break;
	case WeaponType::Rifle:
		player->playerFire->EquipPrimaryWeapon();

		break;
	}

}

WeaponType UWeaponUI::FindSelectWeaponType()
{
	if (IsValid(player) == false) return WeaponType::None;

	UPlayerMove* pMove = Cast<UPlayerMove>(player->playerMove);

	float vertical = pMove->turnValue;
	float horizontal = pMove->lookUpValue;

	if (vertical > 5)
	{
		if (horizontal < -5)
			return WeaponType::Rifle;
		else if (horizontal > 5)
			return WeaponType::Shotgun;
	}
	else if (vertical < -5)
	{
		if (horizontal <  -5)
			return WeaponType::Bow;
		else if (horizontal > 5)
			return WeaponType::Pistol;
	}

	return WeaponType::Sword;

}
