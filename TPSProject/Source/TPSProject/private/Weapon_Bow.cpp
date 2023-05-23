// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Bow.h"

AWeapon_Bow::AWeapon_Bow()
{
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weaponmeshcomp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/BowsAndCrossbowsPack/Assets/Models/LongBow_01/SK_LongBow_01.SK_LongBow_01'"));

	if (tempMesh.Succeeded())
	{
		weaponMeshComp->SkeletalMesh = tempMesh.Object;
	}

	RootComponent = weaponMeshComp;

	pickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("pickupCollision"));

	pickupCollision->SetRelativeLocation(FVector(0, 0, 10));
	pickupCollision->SetRelativeScale3D(FVector(1, 1, 2.25));
	pickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	pickupCollision->SetupAttachment(RootComponent);

	weaponType = WeaponType::Bow;
	attachCharacterSocketName = FName("LeftHandSocket");
	ammoType = AmmoType::None;
	weaponSlotType = WeaponSlotType::None;
	fireDelay = 1.f;
}

void AWeapon_Bow::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	Super::SynchronizeWhitPlayer(player);
	SetActorRelativeLocation(FVector(5.688160, -1.862145, 3.300226));
	SetActorRelativeRotation(FRotator(-6.309163, -105.981231, -0.805839));
}

void AWeapon_Bow::Attack()
{

}

void AWeapon_Bow::Attack(bool isPress)
{
	FName section = "";
	if (isPress)
	{
		section =  DrawBow;
		bowState = BowState::AimMode;
	}
	else
	{
		section = DrawCancel;
		bowState = BowState::TravelMode;
	}
	myPlayer->PlayMontageInServer(BowActiveAM, section);
}
