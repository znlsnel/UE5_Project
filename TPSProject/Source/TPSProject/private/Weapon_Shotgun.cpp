// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Shotgun.h"

AWeapon_Shotgun::AWeapon_Shotgun()
{
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletalMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Shotgun/Mesh/SKM_Shotgun.SKM_Shotgun'"));

	if (tempMesh.Succeeded())
		weaponMeshComp->SkeletalMesh = tempMesh.Object;

	pickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	pickupCollision->SetRelativeLocation(FVector(0, 9, 0));
	pickupCollision->SetRelativeScale3D(FVector(0.5, 1.3, 0.6));
	pickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	RootComponent = pickupCollision;
	weaponMeshComp->SetupAttachment(RootComponent);

	weaponType = WeaponType::Shotgun;
	ammoType = AmmoType::ShotgunAmmo;
	weaponSlotType = WeaponSlotType::PrimarySlot;
	fireDelay = 0.4;
	FireBulletCount = 1;
	FireSpread = 2.0f;
	attachCharacterSocketName = FName("weapon_r_pistol");


}

void AWeapon_Shotgun::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	Super::SynchronizeWhitPlayer(player);
	SetActorRelativeLocation(FVector(13.5, 1.38, 1.13));
}
