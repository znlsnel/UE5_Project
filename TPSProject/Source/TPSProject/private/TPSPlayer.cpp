// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include "ScreenUI.h"
#include "Weapon_Pistol.h"
#include "Weapon_Rifle.h"
#include "Weapon_Shotgun.h"
#include "Weapon.h"
#include "FootIkActorComponent.h"
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "PickupManager.h"
#include "PlayerUI.h"
#include "StoreUI.h"
#include "StoreActor.h"
#include "TPSPlayerController.h"
#include "Inventory.h"
#include "StoreUI.h"
#include "Weapon_Gun.h"
#include "Weapon_Bow.h"
#include "BuildableItem.h"
#include "TPSProjectGameModeBase.h"
#include "ItemFactoryComp.h"
#include "PlayerAbilityComp.h"
#include "AbilityUpgradeWidget.h"
#include "DamageWidget.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include <Blueprint/UserWidget.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/SkeletalMeshComponent.h>
#include <Blueprint/UserWidget.h>
#include <Net/UnrealNetwork.h>
#include <Logging/LogVerbosity.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	GetWorld()->Exec(GetWorld(), TEXT("DisableAllScreenMessages"));
	Tags.Add("Player");
	UE_SET_LOG_VERBOSITY(LogTemp, NoLogging);
	//SubObjects
	{

		playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
		playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
		IKFootComp = CreateDefaultSubobject<UFootIkActorComponent>(TEXT("IKFootComp"));

		pickupManager = CreateDefaultSubobject<UPickupManager>(TEXT("PickUpManager"));

		playerUI = CreateDefaultSubobject<UPlayerUI>(TEXT("UI"));
	}



	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.15), FRotator(0, -90, 0));

	}

	//OnComponentBeginOverlap.AddDynamic(this, &AGun::BeginOverlap);

// Spring Arm Comp
	{
		springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

		// RootComponent 는 계층구조상 캡슐 콜리전 컴포넌트를 의미함
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->SetRelativeLocation(FVector(0, 30, 70));
		springArmComp->TargetArmLength = 250;
		springArmComp->bUsePawnControlRotation = true;
	}

	// TPS Cam Comp
	{
		tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
		tpsCamComp->SetupAttachment(springArmComp);
		tpsCamComp->bUsePawnControlRotation = false;
		//tpsCamComp->bConstrainAspectRatio = true;
		//tpsCamComp->SetAspectRatio(1.777f);
	}

	// Animation Mode
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}


	// Self
	{
		bUseControllerRotationYaw = true;

		// CharactorMovement
		{
			JumpMaxCount = 2;
		}
	}

	myController = Cast<ATPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	itemFactory = CreateDefaultSubobject<UItemFactoryComp>(TEXT("itemFactory"));
	abilityComp = CreateDefaultSubobject<UPlayerAbilityComp>(TEXT("Abilitys"));
}

ATPSPlayer::~ATPSPlayer()
{
	ATPSPlayer::playerId = 0;

}



void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->Exec(GetWorld(), TEXT("DisableAllScreenMessages"));
	hp = maxHp;
	if (playerUI->screenUI)
		playerUI->screenUI->UpdateScreenUI();

	playerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	HpRecoveryLoop();


	for (int i = 0; i < damageWidgetCount; i++)
	{
		UDamageWidget* tempDamageWidget = CreateWidget<UDamageWidget>(GetWorld(), damageWidgetFactory);

		if (tempDamageWidget) {
			tempDamageWidget->AddToViewport();
			damageWidgets.Add(tempDamageWidget);
		}


			
	}

}

void ATPSPlayer::StartGame()
{
	if (playerFire->currWeapon == nullptr) return;
	playerFire->EquipWeapon(playerFire->currWeapon->weaponType);
}

void ATPSPlayer::GetMineralGrace(int mineral, int grace)
{
	int additionalValue = abilityComp->GetSkillInfo(SkillType::CoinBoost)->powerValue;
	if (additionalValue > 0) {
		mineral *= additionalValue / 100;
		grace *= additionalValue / 100;
	}
	this->Mineral += mineral;
	this->Grace += grace;
}

void ATPSPlayer::UpgradeHp(int addHp)
{
	int RecoveryHp = maxHp;
	maxHp = initHp + addHp;
	RecoveryHp = maxHp - RecoveryHp;
	hp += RecoveryHp;
	playerUI->screenUI->UpdateScreenUI();
}


// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	tickUpdateFunctions.Broadcast();

}

void ATPSPlayer::HpRecoveryLoop()
{
	GetWorldTimerManager().ClearTimer(hpRecoveryTimer);

	FSkillInfo* tempSkillInfo =  abilityComp->GetSkillInfo(SkillType::HpNaturalHealing);
	if (hp < maxHp && tempSkillInfo->point > 0) {
		hp += tempSkillInfo->powerValue;
		if (hp > maxHp) hp = maxHp;
		playerUI->screenUI->UpdateScreenUI();
	}

	GetWorldTimerManager().SetTimer(hpRecoveryTimer, this, &ATPSPlayer::HpRecoveryLoop, 5.f, false);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	onInputBindingDelegate.Broadcast(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("InteractionObject"), EInputEvent::IE_Pressed, this, &ATPSPlayer::InteractionObject);

	PlayerInputComponent->BindAction(TEXT("AbilityWidget"), EInputEvent::IE_Pressed, this, &ATPSPlayer::AbilityWidget);

}

void ATPSPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ATPSPlayer::SetPlayerMouse(bool Active)
{
	playerUI->ToggleMouse(Active);
}


void ATPSPlayer::PlayMontage(UAnimMontage* AM, FName section)
{
	if (playerAnim == nullptr)
		playerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	if (playerAnim)
	{
		if (section != "")
		{
			playerAnim->Montage_Play(AM);
			playerAnim->Montage_JumpToSection(section, AM);

		}
		else
			playerAnim->Montage_Play(AM);
	}
}

bool ATPSPlayer::IsPlayingMontage(UAnimMontage* AM)
{
	if (playerAnim == nullptr)
		playerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	if (playerAnim)
	{
		return playerAnim->Montage_IsPlaying(AM);
	}
	return false;
}



void ATPSPlayer::AddItemInServer(AItem* item)
{
	if (IsValid(item))
	{
		item->myPlayer = this;
		GetInventory()->AddItemToInventory(item);
	}
}

void ATPSPlayer::UpdateAttackAndHp(bool updateAttack, float value)
{
	if (updateAttack)
	{
		AdditionalAttackPower = value;
	}
	else
	{
		hp += 10;
		maxHp += 10;
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("HP : %d"), hp));

		playerUI->screenUI->UpdateScreenUI();
	}
}

UStoreUI* ATPSPlayer::GetStore()
{

	return myController->storeActor->storeUI;
}


int ATPSPlayer::playerId = 0;
const int ATPSPlayer::GetPlayerId()
{
	return playerId++;
}

void ATPSPlayer::InteractionObject()
{
	if (InteractionDelegate.IsBound())
	{

		InteractionDelegate.Execute();
	}
}

void ATPSPlayer::AbilityWidget()
{
	if (myAbilityWidget == nullptr) {
		myAbilityWidget = CreateWidget<UAbilityUpgradeWidget>(GetWorld(), abilityWidgetFactory);
		myAbilityWidget->AddToViewport();
	}

	if (myAbilityWidget) {
		if (myAbilityWidget->isOpen)
			myAbilityWidget->CloseWidget();
		else
			myAbilityWidget->OpenWidget();
	}
}

UDamageWidget* ATPSPlayer::GetDamageWidget()
{
	for (auto damageWidget : damageWidgets) {

		if (damageWidget->isInView == false)
			return damageWidget;
	}
	UDamageWidget* tempWidget = damageWidgets[damageWidgetIndex++];
	if (damageWidgetIndex >= damageWidgetCount)
		damageWidgetIndex = 0;
	return tempWidget;
}





UInventory* ATPSPlayer::GetInventory()
{
	return playerUI->screenUI->inventory;
}

void ATPSPlayer::AddHP(int value)
{
	hp += value;
	if (hp > maxHp) hp = maxHp;
	playerUI->screenUI->UpdateScreenUI();

}





void ATPSPlayer::OnHitEvent(int damage)
{
	int randDamage = UKismetMathLibrary::RandomIntegerInRange(FMath::Max(1, damage - (damage / 3)), damage + (damage / 3));

	if (hp <= 0) return;



	int temphp = hp - randDamage;
	hp = FMath::Max(temphp, 0);
	if (playerUI->screenUI)
		playerUI->screenUI->UpdateScreenUI();

	if (hp <= 0)
	{

		if (IsLocallyControlled())
			OnGameOver();
		isDie = true;

	}

}


void ATPSPlayer::BuyItem(int32 itemId, int ItemGrace, int ItemMineral, int32 ItemCount)
{
	isBought = false;

	if (itemId < 100) ItemCount = 1;

	if (Grace < ItemGrace * ItemCount || Mineral < ItemMineral * ItemCount)
		return;

	if (itemId < 100)
	{
		switch (itemId)
		{
			// Sword
		case 0:
			if (playerFire->weapon_Sword == nullptr) {
				playerFire->SetWeapon(WeaponType::Sword, true);
				isBought = true;
			}
			break;

			// Pistol
		case 1:
			if (playerFire->weapon_Pistol == nullptr) {
				playerFire->SetWeapon(WeaponType::Pistol, true);
				isBought = true;
			}
			break;

			// Rifle
		case 2:
			if (playerFire->weapon_Rifle == nullptr) {
				playerFire->SetWeapon(WeaponType::Rifle, true);
				isBought = true;
			}
			break;

			//Shotgun
		case 3:
			if (playerFire->weapon_Shotgun == nullptr) {
				playerFire->SetWeapon(WeaponType::Shotgun, true);
				isBought = true;
			}
			break;

			//Bow
		case 4:
			if (playerFire->weapon_Bow == nullptr) {
				playerFire->SetWeapon(WeaponType::Bow, true);
				isBought = true;
			}
			break;
		}

	}
	else if (itemId < 1000)
	{

		switch (itemId)
		{
			// Pistol Ammo
		case 101:
			if (playerFire->weapon_Pistol) {
				playerFire->weapon_Pistol->Ammo += (12 * ItemCount);
				isBought = true;
			}
			break;

			// Rifle Ammo
		case 102:
			if (playerFire->weapon_Rifle) {
				playerFire->weapon_Rifle->Ammo += (30 * ItemCount);
				isBought = true;
			}
			break;

			//Shotgun Ammo
		case 103:
			if (playerFire->weapon_Shotgun) {
				playerFire->weapon_Shotgun->Ammo += (8 * ItemCount);
				isBought = true;
			}
			break;

			//Bow Ammo
		case 104:
			if (playerFire->weapon_Bow) {
				playerFire->weapon_Bow->currAmmo += (20 * ItemCount);
				isBought = true;
			}
			break;
		}
	}


	else if (itemId < 10000)
	{
		for (int i = 0; i < ItemCount; i++)
		{
			switch (itemId)
			{

				// 돌담
			case 1000: {
				ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->StoneWall)));
				isBought = true;
			}
				 break;

				 // 바리케이드
			case 1001: {
				ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Barricade)));
				isBought = true;
			}
				 break;

				 // 부서진담
			case 1002: {
				ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->BrokenWalll)));
				isBought = true;
			}
				 break;

				 // 모래주머니 담
			case 1003: {
				ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Sandbag)));
				isBought = true;
			}
				 break;

				 // 터렛
			case 1101: {

				ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Turret)));
				isBought = true;
			}
				 break;
			}
		}
		GetWorldTimerManager().ClearTimer(addItemTimer);
		GetWorld()->GetTimerManager().SetTimer(addItemTimer, FTimerDelegate::CreateLambda([&]() {
				while (ItemArr.Num() > 0)
				{
					auto item = ItemArr.Pop();
					item->myPlayer = this; 
					GetInventory()->AddItemToInventory(item);
				}
			}), 1.f, false);
	}

	if (isBought) {
		Grace -= ItemGrace * ItemCount;
		Mineral -= ItemMineral * ItemCount;
	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);


}



