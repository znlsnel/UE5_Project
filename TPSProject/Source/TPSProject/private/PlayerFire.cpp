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

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathVectorCommon.h>

UPlayerFire::UPlayerFire()
{
	// 총알 사운드
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
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &UPlayerFire::LoadBullet);
	PlayerInputComponent->BindAction(TEXT("GetPistol"), IE_Pressed, this, &UPlayerFire::EquipSecondaryWeapon);
	PlayerInputComponent->BindAction(TEXT("GetRiple"), IE_Pressed, this, &UPlayerFire::EquipPrimaryWeapon);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Pressed, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Released, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();
	//  스나이퍼 UI 위젯 인스턴스  생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 일반 조준 크로스헤어 UI
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// 일반 조준 UI 등록
	_crosshairUI->AddToViewport();


	controller = GetWorld()->GetFirstPlayerController();

	me->OnInitialization();

	InitializeWeapon();
	EquipSecondaryWeapon();
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UPlayerFire::InputFire()
{
	if (me->playerUI->isInventoryOpen())
	{
		me->playerUI->GetMouseInput();
		return;
	}
	AWeapon* tempWeapon = GetWeapon();
	if (tempWeapon) tempWeapon->Attack();

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

void UPlayerFire::SniperAim()
{
	if (bUsingPistol == true) return;

	// Pressed 입력 처리
	if (bSniperAim == false)
	{
		// 스나이퍼 조준 모드 활성화!
		bSniperAim = true;
		// 스나이퍼 조준 UI 등록
		_sniperUI->AddToViewport();
		me->tpsCamComp->SetFieldOfView(45.0f);
		_crosshairUI->RemoveFromParent();
	}
	// Released 입력 처리
	else
	{
		// 스나이퍼 조준 모드 비활성화!
		bSniperAim = false;
		// 스나이퍼 조준 UI 등록
		_sniperUI->RemoveFromParent();
		me->tpsCamComp->SetFieldOfView(90.0f);
		_crosshairUI->AddToViewport();
	}
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

}

