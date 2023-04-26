// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerFire.h"
#include "Weapon.h"
#include "Weapon_Pistol.h"
#include "Weapon_Rifle.h"
#include "Weapon_Shotgun.h"
#include "ScreenUI.h"
#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "EnemyFSM.h"
#include "Bullet.h"
#include "PlayerUI.h"
#include "Crosshair.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathVectorCommon.h>

UPlayerFire::UPlayerFire()
{
	// ÃÑ¾Ë »ç¿îµå
	{
		ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/Assets/Sounds/Rifle.Rifle'"));
		if (tempSound.Succeeded())
		{
			bulletSound = tempSound.Object;
		}
	}
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire<true>);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &UPlayerFire::InputFire<false>);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &UPlayerFire::LoadBullet);
	PlayerInputComponent->BindAction(TEXT("GetPistol"), IE_Pressed, this, &UPlayerFire::EquipSecondaryWeapon);
	PlayerInputComponent->BindAction(TEXT("GetRiple"), IE_Pressed, this, &UPlayerFire::EquipPrimaryWeapon);

	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Pressed, this, &UPlayerFire::SniperAim<true>);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Released, this, &UPlayerFire::SniperAim<false>);
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	controller = GetWorld()->GetFirstPlayerController();

	me->OnInitialization();

	InitializeWeapon();
	EquipSecondaryWeapon();
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}



void UPlayerFire::InputFire(bool isPressed)
{
	isFire = isPressed;
	LoopFire();
}

void UPlayerFire::LoopFire()
{
	if (isFire == false) return;

	if (me->playerUI->isInventoryOpen())
	{
		me->playerUI->GetMouseInput();
		return;
	}
	AWeapon* tempWeapon = GetWeapon();
	if (tempWeapon) tempWeapon->Attack();
	me->playerUI->crosshair->AttackCrosshair();

	GetWorld()->GetTimerManager().SetTimer(fireTimerHandle, this, &UPlayerFire::LoopFire, GetWeapon()->fireDelay);
}

void UPlayerFire::LoadBullet()
{
	AWeapon* tempWeapon = GetWeapon();
	if (tempWeapon) tempWeapon->Reload();
}

void UPlayerFire::EquipSecondaryWeapon()
{
	if (secondaryWeapon == nullptr) return;
	
	currSlot = WeaponSlotType::SecondarySlot;
	currWeapon = secondaryWeapon->weaponType;
	EquipWeapon(currSlot);
}

void UPlayerFire::EquipPrimaryWeapon()
{
	if (primaryWeapon == nullptr) return;

	currSlot = WeaponSlotType::PrimarySlot;
	currWeapon = primaryWeapon->weaponType;
	EquipWeapon(currSlot);
}

void UPlayerFire::SniperAim(bool isPressed)
{
	if (isPressed)
		me->tpsCamComp->SetFieldOfView(30.0f);
	else
		me->tpsCamComp->SetFieldOfView(90.0f);
}

void UPlayerFire::InitializeWeapon()
{
	secondaryWeapon = GetWorld()->SpawnActor<AWeapon_Pistol>(pistol);
	secondaryWeapon->SynchronizeWhitPlayer(me);

	//primaryWeapon = GetWorld()->SpawnActor<AWeapon_Rifle>(rifle);
	//primaryWeapon->SynchronizeWhitPlayer(me);

	currWeapon = WeaponType::Pistol;
	currSlot = WeaponSlotType::SecondarySlot;
	EquipWeapon(currSlot);
}



AWeapon* UPlayerFire::GetWeapon()
{
	switch (currSlot)
	{
	case WeaponSlotType::PrimarySlot:
		return primaryWeapon;
	case WeaponSlotType::SecondarySlot:
		return secondaryWeapon;
	}
	return nullptr;
}

void UPlayerFire::EquipWeapon(WeaponSlotType slotType)
{
	switch (slotType)
	{
	case WeaponSlotType::PrimarySlot:
		if (primaryWeapon) primaryWeapon->UncoverWeapon();
		if (secondaryWeapon) secondaryWeapon->HideWeapon();
		break;
	case WeaponSlotType::SecondarySlot:
		if (secondaryWeapon) secondaryWeapon->UncoverWeapon();
		if (primaryWeapon) primaryWeapon->HideWeapon();
		break;
	}
	if (me->playerUI->screenUI) me->playerUI->screenUI->WeaponSwap();
	UPlayerAnim* anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());

	anim->weaponType = currWeapon;
	anim->weaponSlotType = currSlot;
	me->playerUI->crosshair->ChangeCrosshair(currWeapon);

}

