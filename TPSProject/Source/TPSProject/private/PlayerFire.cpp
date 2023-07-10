// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerFire.h"
#include "Weapon.h"
#include "Weapon_Pistol.h"
#include "Weapon_Rifle.h"
#include "Weapon_Shotgun.h"
#include "Weapon_Bow.h"
#include "Weapon_Sword.h"
#include "ScreenUI.h"
#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "EnemyFSM.h"
#include "PlayerUI.h"
#include "Crosshair.h"
#include "TPSPlayerController.h"
#include "StoreUI.h"
#include "BuildableItem.h"
#include "PlayerAbilityComp.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathVectorCommon.h>
#include <GameFramework/PlayerState.h>


UPlayerFire::UPlayerFire()
{
	SetIsReplicated(true);
	// ÃÑ¾Ë »ç¿îµå
	{
		ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/Assets/Sounds/Rifle.Rifle'"));
		if (tempSound.Succeeded())
		{
			bulletSound = tempSound.Object;
		}
	}
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire<true>);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &UPlayerFire::InputFire<false>);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &UPlayerFire::LoadBullet);

	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Pressed, this, &UPlayerFire::SniperAim<true>);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Released, this, &UPlayerFire::SniperAim<false>);
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	controller = GetWorld()->GetFirstPlayerController();
	//controller = me->GetPlayerState()->GetPlayerController();

	me->OnInitialization();
	anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());

	//GetWorld()->GetTimerManager().SetTimer(BeginPlayTimer, FTimerDelegate::CreateLambda(
	//	[&]() {
	//		SetWeapon(WeaponType::Sword); 
	//	}
	//),  1.f, false);

	SetWeapon(WeaponType::Sword, false);

//	SetWeapon(WeaponType::Pistol);
}


void UPlayerFire::InputFire(bool isPressed)
{
	if (me->isDie) return;
	isFire = isPressed;
	if (me->abilityComp->GetMouseInput()) return;

	if (currWeapon == nullptr)return;
	if (IsValid(me->buildableItem) && me->buildableItem->isBuild)
	{
		me->buildableItem->GetMouseInput(isPressed);
		return;
	}
	
	if (currWeapon->weaponType == WeaponType::Sword) {
		if (Cast<AWeapon_Sword>(currWeapon)->isBlocking == true)
			return;
	}

	if (!isFire)
	{
		me->playerUI->crosshair->AttackCrosshair(isFire);
	}

	if (currWeapon->weaponType == WeaponType::Bow)
		Cast<AWeapon_Bow>(currWeapon)->Attack(isFire);
	else
		LoopFire();
	
}

void UPlayerFire::LoopFire()
{

	if (isFire == false) return;

	if (lastShotTime == 0.f)
		lastShotTime = GetWorld()->GetTimeSeconds();

	else if (GetWorld()->GetTimeSeconds() - lastShotTime < currWeapon->fireDelay - 0.1f)
	{
		return;
	}
	else
		lastShotTime = GetWorld()->GetTimeSeconds();



	if (me->playerUI->IsMouseActive)
	{
		me->playerUI->GetMouseInput();
		return;
	}

	if (currWeapon) currWeapon->Attack();
	me->playerUI->crosshair->AttackCrosshair(isFire);

	GetWorld()->GetTimerManager().SetTimer(fireTimerHandle, this, &UPlayerFire::LoopFire, currWeapon->fireDelay);
}

void UPlayerFire::LoadBullet()
{
	if (currWeapon) currWeapon->Reload();
}


void UPlayerFire::SniperAim(bool isPressed)
{
	if (me->isDie) return;
	if (currWeapon && currWeapon->ActorHasTag(TEXT("Sword"))) {
		if (isPressed) {
			Cast<AWeapon_Sword>(currWeapon)->BlockAttack();
		}
		return;
	}

	if (isPressed) {
		zoomCameraValue = 45.f;
		zoomCamera();
	}
		/*me->tpsCamComp->SetFieldOfView(45.0f);*/
	else {
		zoomCameraValue = 90.f;
		zoomCamera();
	}

		//me->tpsCamComp->SetFieldOfView(90.0f);
}

void UPlayerFire::zoomCamera()
{
	GetWorld()->GetTimerManager().ClearTimer(ZoomTimer);
	if (me->tpsCamComp->FieldOfView == zoomCameraValue|| me->isDie) {
		return;
	}

	me->tpsCamComp->SetFieldOfView(FMath::FInterpTo(me->tpsCamComp->FieldOfView, zoomCameraValue,GetWorld()->GetDeltaSeconds(), 3.f));

	GetWorld()->GetTimerManager().SetTimer(ZoomTimer, this, &UPlayerFire::zoomCamera, 0.01f, false);
}


void UPlayerFire::EquipWeapon(WeaponType weaponType)
{
	nextWeapon = GetWeapon(weaponType);
	if (IsValid(nextWeapon)) {
		me->PlayMontage(nextWeapon->CharacterEquipAM);
	}
	
}


int UPlayerFire::GetOwnWeapons()
{
	int result = 0;

	if (IsValid(weapon_Pistol))
		result |= 1;

	if (IsValid(weapon_Rifle))
		result |= 1 << 1;

	if (IsValid(weapon_Shotgun))
		result |= 1 << 2;

	if (IsValid(weapon_Bow))
		result |= 1 << 3;

	return result;
}

void UPlayerFire::SetOwnWeapons(int ownWeapon)
{
	EquipWeapon(WeaponType::Sword);

	// pistol
	if (ownWeapon & 1) {
		SetWeapon(WeaponType::Pistol, false);
	}
	else weapon_Pistol = nullptr;

	//rifle
	if (ownWeapon & (1 << 1)) {
		SetWeapon(WeaponType::Rifle , false);
	}
	else weapon_Rifle = nullptr;

	// shogun
	if (ownWeapon & (1 << 2)) {
		SetWeapon(WeaponType::Shotgun, false);
	}
	else weapon_Shotgun = nullptr;

	if (ownWeapon & (1 << 3)) {
		SetWeapon(WeaponType::Bow, false);
	}
	else weapon_Bow = nullptr;
	//bow

}


void UPlayerFire::ChangeWeapon(AWeapon* weapon)
{
	if (weapon)
		nextWeapon = weapon;

	if (currWeapon)
		currWeapon->HideWeapon();
	if (nextWeapon == nullptr) return;
	nextWeapon->UncoverWeapon();
	currWeapon = nextWeapon;

	UScreenUI* screenUI = me->playerUI->screenUI;
	if (screenUI) screenUI->WeaponSwap();

	anim->weaponType = currWeapon->weaponType;
	me->playerUI->crosshair->ChangeCrosshair(currWeapon->weaponType);
}

AWeapon* UPlayerFire::GetWeapon(WeaponType weaponType)
{
	switch (weaponType)
	{
	case WeaponType::Rifle:
		return weapon_Rifle;
		break;
	case WeaponType::Shotgun:
		return weapon_Shotgun;
		break;
	case WeaponType::Pistol: 
		return weapon_Pistol;
		break;
	case WeaponType::Bow:
		return weapon_Bow;
		break;
	case WeaponType::Sword:
		return weapon_Sword;
		break;
	}
	return nullptr;
}


void UPlayerFire::SetWeapon(AWeapon* weapon, bool equipWeapon)
{
	if (weapon == nullptr)
		return;
	
	switch (weapon->weaponType)
	{
	case WeaponType::Pistol:
		weapon_Pistol = weapon;
		break;
	case WeaponType::Rifle:
		weapon_Rifle = weapon;
		break;
	case WeaponType::Shotgun:
		weapon_Shotgun = weapon;
		break;
	case WeaponType::Bow:
		weapon_Bow = weapon;
		 break;
	case WeaponType::Sword:
		weapon_Sword = weapon;
		break;
	}

	weapon->SynchronizeWhitPlayer(me);
	weapon->HideWeapon();
	if (equipWeapon) 
		EquipWeapon(weapon->weaponType);
	

}


void UPlayerFire::SetWeapon(WeaponType weaponType, bool equipWeapon)
{
	AWeapon* weapon = nullptr;
	switch (weaponType)
	{
	case WeaponType::Pistol:
		weapon = GetWorld()->SpawnActor<AWeapon_Pistol>(pistol);
		break;

	case WeaponType::Rifle:
		weapon = GetWorld()->SpawnActor<AWeapon_Rifle>(Rifle);
		break;

	case WeaponType::Shotgun:
		weapon = GetWorld()->SpawnActor<AWeapon_Shotgun>(Shotgun);
		break;

	case WeaponType::Bow:
		weapon = GetWorld()->SpawnActor<AWeapon_Bow>(Bow);
		break;

	case WeaponType::Sword:
		weapon = GetWorld()->SpawnActor<AWeapon_Sword>(Sword);
		break;
	}

	if (IsValid(weapon))
		SetWeapon(weapon, equipWeapon);
	

}