// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Pistol.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

void AWeapon_Pistol::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	Super::SynchronizeWhitPlayer(player);
	SetActorRelativeLocation(FVector(-8.355, 1.8866, 0.3986));
	SetActorRelativeRotation(FRotator(-23.35, 101.85, -14.97));
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
	RootComponent = pickupCollision;
	weaponMeshComp->SetupAttachment(RootComponent);

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


