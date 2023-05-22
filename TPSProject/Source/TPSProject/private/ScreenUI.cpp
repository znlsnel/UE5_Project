// Fill out your copyright notice in the Description page of Project Settings.
#include "ScreenUI.h"
#include "PlayerFire.h"
#include "TPSPlayer.h"
#include "Inventory.h"
#include "Weapon.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Components/Image.h>

#include <GameFramework/PlayerState.h>






void UScreenUI::UpdateScreenUI()
{
	hpRatio = static_cast<float>(myPlayer->hp) / static_cast<float>(myPlayer->initialHp);
	hpInfo = FString::Printf(TEXT("%d / %d"), myPlayer->hp, myPlayer->initialHp);
}

void UScreenUI::ATVWidget()
{
	if (IsInViewport()) return;
	if (myPlayer->GetNetMode() == NM_Client && myPlayer->GetLocalRole() == ROLE_AutonomousProxy)
		StartWidget();

	WeaponSwap();
}

void UScreenUI::Initialization(ATPSPlayer* player)
{
	if (IsValid(this) == false) return;
	myPlayer = player;

	inventory = CreateWidget<UInventory>(GetWorld());
	inventory->Initialization(myPlayer);
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

		bOpenInventory = false;
	}
	else
	{

		bOpenInventory = true;
	}

	BP_ToggleInventory();
}

FString UScreenUI::UpdateAmmoCount()
{
	AWeapon* weapon = myPlayer->playerFire->currWeapon;
	FString ammoString = FString::Printf(TEXT("%d / %d"), weapon->currAmmo, weapon->Ammo);



	return ammoString;
}



void UScreenUI::WeaponSwap_Implementation()
{

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
