// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "TPSPlayer.h"
#include "PlayerFire.h"
#include "Weapon.h"

#include <Components/BoxComponent.h>

AAmmoBox::AAmmoBox()
{
	SetReplicates(true);
	itemType = ItemType::Ammo;

	pickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	pickupCollision->SetRelativeLocation(FVector(0, 9, 0));
	pickupCollision->SetRelativeScale3D(FVector(0.5, 1.3, 1.2));
	pickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

bool AAmmoBox::UseAmmoBox()
{
	AWeapon* weapon = myPlayer->playerFire->currWeapon;

	if (ammoType == weapon->ammoType)
	{
		weapon->Ammo += ammo;
		return true;
	}

	return false;
}
