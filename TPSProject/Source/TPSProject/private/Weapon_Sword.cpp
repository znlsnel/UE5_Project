// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Sword.h"

AWeapon_Sword::AWeapon_Sword()
{
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weaponmeshcomp"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_SwordB/SK_Blade_SwordB.SK_Blade_SwordB'"));

	if (tempMesh.Succeeded())
	{
		weaponMeshComp->SkeletalMesh = tempMesh.Object;
	}

	RootComponent = weaponMeshComp;

	pickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("pickupCollision"));

	pickupCollision->SetRelativeLocation(FVector(0, 0, 40));
	pickupCollision->SetRelativeScale3D(FVector(1, 1, 2.25));
	pickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	pickupCollision->SetupAttachment(RootComponent);
	weaponType = WeaponType::Sword;
	attachCharacterSocketName = FName("RightHandSocket");
}

void AWeapon_Sword::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	Super::SynchronizeWhitPlayer(player);
	SetActorRelativeLocation(FVector(-4.195, 2.5516, 2.2886));
	SetActorRelativeRotation(FRotator(-21.61, 90.65, 21.37));
}

void AWeapon_Sword::Attack()
{

	FName attackSection = "";
	if (myPlayer->GetMovementComponent()->IsFalling())
	{
		currCombo = 0;
		attackSection = Attack_Air;
	}
	else
	{
		if (lastAttackTime == 0.f){
			lastAttackTime = GetWorld()->GetTimeSeconds();
			currCombo = 0;
		}
		else if (GetWorld()->GetTimeSeconds() - lastAttackTime < nextComboDelay){
			++currCombo;
			if (currCombo > 3) 
				currCombo = 0;
			lastAttackTime = GetWorld()->GetTimeSeconds();
		}

		else{
			lastAttackTime = 0.f;
			currCombo = 0;
		}


		switch (currCombo)
		{
		case 0:
			attackSection = Attack_1;
			break;
		case 1:
			attackSection = Attack_2;
			break;
		case 2:
			attackSection = Attack_3;
			break;
		case 3:
			attackSection = Attack_4;
			break;
		}
	}
	UKismetSystemLibrary::PrintString(GetWorld(), attackSection.ToString());


	myPlayer->PlayMontageInServer(CharacterFireAM, attackSection);
}
