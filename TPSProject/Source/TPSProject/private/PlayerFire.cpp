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

	GetWorld()->GetTimerManager().SetTimer(BeginPlayTimer, FTimerDelegate::CreateLambda(
		[&]() {SetWeapon(WeaponType::Pistol); }
	),  1.f, false);

//	SetWeapon(WeaponType::Pistol);
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}


void UPlayerFire::InputFire(bool isPressed)
{
	isFire = isPressed;
	if (currWeapon == nullptr)return;
	if (IsValid(me->buildableItem) && me->buildableItem->isBuild)
	{
		me->buildableItem->GetMouseInput(isPressed);
		return;
	}
		
	me->ClickBPWidget(isFire);
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
	if (isPressed)
		me->tpsCamComp->SetFieldOfView(45.0f);
	else
		me->tpsCamComp->SetFieldOfView(90.0f);
}


void UPlayerFire::EquipWeapon(WeaponType weaponType)
{
	nextWeapon = GetWeapon(weaponType);
	if (IsValid(nextWeapon)) {
		me->PlayMontage(nextWeapon->CharacterEquipAM);
	}
	
}

void UPlayerFire::ChangeWeapon()
{
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