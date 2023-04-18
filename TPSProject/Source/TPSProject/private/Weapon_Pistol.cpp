// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Pistol.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

void AWeapon_Pistol::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	Super::SynchronizeWhitPlayer(player);
	SetActorRelativeLocation(FVector(-7.0, 3.8, -0.5));
}

AWeapon_Pistol::AWeapon_Pistol()
{
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));

	if (tempMesh.Succeeded())
		weaponMeshComp->SkeletalMesh = tempMesh.Object;

	pickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	pickupCollision->SetRelativeLocation(FVector(0, 9, 0));
	pickupCollision->SetRelativeScale3D(FVector(0.5, 1.3, 0.6));
	pickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	weaponMeshComp->SetupAttachment(pickupCollision);

	weaponType = WeaponType::Pistol;
	ammoType = AmmoType::PistolAmmo;
	weaponSlotType = WeaponSlotType::SecondarySlot;
	fireDelay = 0.4;
	FireBulletCount = 1;
	FireSpread = 2.0f;
	attachCharacterSocketName = FName("weapon_r_pistol");



}

void AWeapon_Pistol::Attack()
{
	Super::Attack();


}


