// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Rifle.h"

AWeapon_Rifle::AWeapon_Rifle()
{
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletalMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Rifle/Mesh/SK_Rifle.SK_Rifle'"));

	if (tempMesh.Succeeded())
		weaponMeshComp->SkeletalMesh = tempMesh.Object;
		
	weaponType = WeaponType::Rifle;
	ammoType = AmmoType::RifleAmmo;
	weaponSlotType = WeaponSlotType::PrimarySlot;
	fireDelay = 0.4;
	FireBulletCount = 1;
	FireSpread = 2.0f;
	attachCharacterSocketName = FName("weapon_r_pistol");
}
