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

	FKey MouseButton = EKeys::LeftMouseButton;
	GetWeapon()->ClickWorldWidget(isFire);

	if (!isFire)
	{
		me->playerUI->crosshair->AttackCrosshair(isFire);
	}

	LoopFire();
	
}

void UPlayerFire::LoopFire()
{
	if (isFire == false) return;
	
	if (lastShotTime == 0.f)
		lastShotTime = GetWorld()->GetTimeSeconds();

	else if (GetWorld()->GetTimeSeconds() - lastShotTime < GetWeapon()->fireDelay - 0.1f)
	{
		return;
	}
	else
		lastShotTime = GetWorld()->GetTimeSeconds();


	if (me->playerUI->isInventoryOpen())
	{
		me->playerUI->GetMouseInput();
		return;
	}
	AWeapon* tempWeapon = GetWeapon();
	if (tempWeapon) tempWeapon->Attack();
	me->playerUI->crosshair->AttackCrosshair(isFire);

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
		me->tpsCamComp->SetFieldOfView(45.0f);
	else
		me->tpsCamComp->SetFieldOfView(90.0f);
}

void UPlayerFire::InitializeWeapon()
{
	secondaryWeapon = GetWorld()->SpawnActor<AWeapon_Pistol>(pistol);
	secondaryWeapon->SynchronizeWhitPlayer(me);

	//primaryWeapon = GetWorld()->SpawnActor<AWeapon_Rifle>(rifle);
	//primaryWeapon->SynchronizeWhitPlayer(me);

	anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());

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
	if (IsValid(anim) == false)
		anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	switch (slotType)
	{
	case WeaponSlotType::PrimarySlot:
		if (primaryWeapon) anim->PlayPlayerMontage(primaryWeapon->CharacterEquipAM);
		break;
	case WeaponSlotType::SecondarySlot:
		if (secondaryWeapon) anim->PlayPlayerMontage(secondaryWeapon->CharacterEquipAM);
		break;
	}

}

void UPlayerFire::ChangeWeapon()
{
	switch (currSlot)
	{
	case WeaponSlotType::PrimarySlot:
	{
		if (primaryWeapon) primaryWeapon->UncoverWeapon();
		if (secondaryWeapon) secondaryWeapon->HideWeapon();
	}
		break;
	case WeaponSlotType::SecondarySlot:
	{
		if (secondaryWeapon) secondaryWeapon->UncoverWeapon();
		if (primaryWeapon) primaryWeapon->HideWeapon();
	}
		break;
	}

	if (me->playerUI->screenUI) me->playerUI->screenUI->WeaponSwap();



	anim->weaponType = currWeapon;
	anim->weaponSlotType = currSlot;
	me->playerUI->crosshair->ChangeCrosshair(currWeapon);

}

