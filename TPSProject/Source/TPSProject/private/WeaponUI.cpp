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

	WeaponType tempType = WeaponType::Sword;
	FVector2D mousePos = FVector2D(vertical, horizontal);

////	float rifleButtonDist = FVector::Dist(mousePos,)
//	if (vertical > 1)
//	{
//		if (horizontal < -1)
//			tempType = WeaponType::Rifle;
//		else if (horizontal > 1)
//			tempType = WeaponType::Bow;
//		else
//			tempType = WeaponType::Sword;
//	}
//	else if (vertical < -1)
//	{
//		if (horizontal < -1)
//			tempType = WeaponType::Shotgun;
//		else if (horizontal > 1)
//			tempType = WeaponType::Pistol;
//		else
//			tempType = WeaponType::Sword;
//	}

	return GetCloseButton(mousePos);

}


float UWeaponUI::Get2DVectorDist(FVector2D& vectorA, FVector2D vectorB)
{
	//float tempA = FMath::Pow(vectorA.Y - vectorA.X, 2.f);
	//float tempB = FMath::Pow(vectorB.Y - vectorB.X, 2.f);

	return  FVector2D::Distance(vectorA, vectorB);

//	return FMath::Sqrt(tempA + tempB);
}

WeaponType UWeaponUI::GetCloseButton(FVector2D& mousePos)
{
	float RifleButtonDist = Get2DVectorDist(mousePos, FVector2D(10, -10));
	float BowButtonDist = Get2DVectorDist(mousePos, FVector2D(10, 10));
	float SwordButtonDist = Get2DVectorDist(mousePos, FVector2D(0, 0));
	float ShotgunButtonDist = Get2DVectorDist(mousePos, FVector2D(-10, -10));
	float PistolButtonDist = Get2DVectorDist(mousePos, FVector2D(-10, 10));

	float min = FMath::Min3(RifleButtonDist, BowButtonDist, FMath::Min3(SwordButtonDist, ShotgunButtonDist, PistolButtonDist));

	if (min == RifleButtonDist)
		return WeaponType::Rifle;
	else if (min == BowButtonDist)
		return WeaponType::Bow;
	else if (min == ShotgunButtonDist)
		return WeaponType::Shotgun;
	else if (min == PistolButtonDist)
		return WeaponType::Pistol;
	else
		return WeaponType::Sword;

}
