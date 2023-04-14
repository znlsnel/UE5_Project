// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Pistol.h"

AWeapon_Pistol::AWeapon_Pistol()
{
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));

	if (tempMesh.Succeeded())
		weaponMeshComp->SkeletalMesh = tempMesh.Object;

	weaponType = WeaponType::Pistol;
	ammoType = AmmoType::PistolAmmo;
	weaponSlotType = WeaponSlotType::SecondarySlot;
	fireDelay = 0.4;
	FireBulletCount = 1;
	FireSpread = 2.0f;
	attachCharacterSocketName = FName("weapon_r_pistol");
}
