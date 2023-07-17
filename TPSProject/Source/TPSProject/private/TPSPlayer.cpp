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
#include "playerHpEffect.h"
#include "DamageWidget.h"
#include "BuildableItemCheckUI.h"
#include "StatueAbilityWidget.h"
#include "ItemStoreUI.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/AudioComponent.h>

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
#include <particles/ParticleSystemComponent.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	Tags.Add("Player");
	Tags.Add("EnemysTarget");
	
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
	IKFootComp = CreateDefaultSubobject<UFootIkActorComponent>(TEXT("IKFootComp"));
	pickupManager = CreateDefaultSubobject<UPickupManager>(TEXT("PickUpManager"));
	playerUI = CreateDefaultSubobject<UPlayerUI>(TEXT("UI"));
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	meleeAttackSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackSensor"));

	itemFactory = CreateDefaultSubobject<UItemFactoryComp>(TEXT("itemFactory"));
	abilityComp = CreateDefaultSubobject<UPlayerAbilityComp>(TEXT("Abilitys"));
	SpawnEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpawnEffect"));
	DamageEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamageEffect"));
	// RootComponent 는 계층구조상 캡슐 콜리전 컴포넌트를 의미함
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 30, 70));
	springArmComp->TargetArmLength = 250;
	springArmComp->bUsePawnControlRotation = true;

	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	meleeAttackSensor->SetupAttachment(GetMesh());
	meleeAttackSensor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	DamageEffect->SetupAttachment(GetMesh());
	DamageEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);


	myController = Cast<ATPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC'"));

	if (TempMesh.Succeeded()){
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.15), FRotator(0, -90, 0));
	}
}

ATPSPlayer::~ATPSPlayer()
{
	ATPSPlayer::playerId = 0;
}



void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	SpawnEffect->Deactivate();
	DamageEffect->Deactivate();

	heartAudio = UGameplayStatics::SpawnSound2D(GetWorld(), heartSound);
	heartAudio->Stop();
	GetWorld()->Exec(GetWorld(), TEXT("DisableAllScreenMessages"));
	hp = maxHp;
	if (playerUI->screenUI)
		playerUI->screenUI->UpdateScreenUI();

	playerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	HpRecoveryLoop();

	BuildableItemCheckUI = CreateWidget<UBuildableItemCheckUI>(GetWorld(), CheckUIFactory);
	myAbilityWidget = CreateWidget<UAbilityUpgradeWidget>(GetWorld(), abilityWidgetFactory);
	//BuildableItemCheckUI->AddToViewport();

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

void ATPSPlayer::GameOver()
{
	playerUI->GameOver();
}


// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	tickUpdateFunctions.Broadcast();

}

void ATPSPlayer::HpRecoveryLoop()
{
	FSkillInfo* tempSkillInfo =  abilityComp->GetSkillInfo(SkillType::HpNaturalHealing);
	if (tempSkillInfo && hp < maxHp && tempSkillInfo->point > 0) {
		AddHP(tempSkillInfo->powerValue);
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

void ATPSPlayer::OpenStatueAbilityWidget()
{
	if (playerUI->statueAbilityWidget->IsInViewport() == false) {

		playerUI->statueAbilityWidget->AddToViewport();
		playerUI->statueAbilityWidget->OpenWidget();

	}
}

void ATPSPlayer::StartSequenceBegin()
{
	SetActorHiddenInGame(true);
}

void ATPSPlayer::StartSequenceEnd()
{
	SetActorHiddenInGame(false);

	if (playerFire->weapon_Sword) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("dd"));
		playerFire->ChangeWeapon(playerFire->weapon_Sword);
	}
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
	if (myAbilityWidget->IsInViewport() == false) {

		myAbilityWidget->AddToViewport();
	}

	if (myAbilityWidget) {
		if (myAbilityWidget->isOpen) {
			myAbilityWidget->CloseWidget();
		}
		else {
			myAbilityWidget->OpenWidget();
		}
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
	playerUI->screenUI->playerHpEvent(true);
	
	if (hp > maxHp / 5) {
		if (heartAudio && heartAudio->IsPlaying()) {
			heartAudio->Stop();
		}
	}
}





void ATPSPlayer::OnHitEvent(int damage, FVector enemyPos)
{
	if (GetWorld()->GetTimeSeconds() - lastHitTime < 0.5f)
		return;
	if (hp <= 0) return;
	if (isDie) return;

	lastHitTime = GetWorld()->GetTimeSeconds();
	int randDamage = UKismetMathLibrary::RandomIntegerInRange(FMath::Max(1, damage - (damage / 3)), damage + (damage / 3));

	randDamage = 1;
	DamageEffect->Activate(true);
	int temphp = hp - randDamage;
	hp = FMath::Max(temphp, 0);

	if (playerUI->screenUI) {
		playerUI->screenUI->UpdateScreenUI();
		playerUI->screenUI->playerHpEvent(false);
	}

	if (hp < maxHp / 5) {
		if (IsValid(heartAudio)) {
			heartAudio->Stop();
			heartAudio->Play();
		}

	}

	if (hp <= 0)
	{
		OnPlayerDie();
		isDie = true;

	}
	FName temp = FName("Front");

	FVector dir = (enemyPos - GetActorLocation()).GetSafeNormal();

	float forward = FVector::DotProduct(GetActorForwardVector(), dir);
	float Right = FVector::DotProduct(GetActorRightVector(), dir);

	float checkForward = FMath::Abs(forward);
	float checkRight = FMath::Abs(Right);

	if (checkForward < checkRight) {
		if (Right > 0.f) {
			temp = TEXT("Right");
		}
		else {
			temp = TEXT("Left");
		}
	}
	else {
		if (forward > 0.f) {
			temp = TEXT("Front");
		}
		else {
			temp = TEXT("Back");
		}
	}

	PlayMontage(AM_damage, temp);
}


void ATPSPlayer::BuyItem(EItemID itemId, int ItemGrace, int ItemMineral, int32 ItemCount, bool& result)
{
	result = false;

	if (Grace < ItemGrace * ItemCount || Mineral < ItemMineral * ItemCount)
		return;

	for (int i = 0; i < ItemCount; i++) {
		switch (itemId) {
		case EItemID::Pistol:
			if (playerFire->weapon_Pistol == nullptr) {
				playerFire->SetWeapon(WeaponType::Pistol, true);
				result = true;
			}
			break;
		case EItemID::Shotgun:
			if (playerFire->weapon_Shotgun == nullptr) {
				playerFire->SetWeapon(WeaponType::Shotgun, true);
				result = true;
			}
			break;
		case EItemID::Rifle:
			if (playerFire->weapon_Rifle == nullptr) {
				playerFire->SetWeapon(WeaponType::Rifle, true);
				result = true;
			}
			break;
		case EItemID::Bow:
			if (playerFire->weapon_Bow == nullptr) {
				playerFire->SetWeapon(WeaponType::Bow, true);
				result = true;
			}
			break;

		case EItemID::PistolAmmo:
			if (playerFire->weapon_Pistol) {
				playerFire->weapon_Pistol->Ammo += (12);
				result = true;
			}
			break;
		case EItemID::ShotgunAmmo:
			if (playerFire->weapon_Shotgun) {
				playerFire->weapon_Shotgun->Ammo += (8);
				result = true;
			}
			break;
		case EItemID::RifleAmmo:
			if (playerFire->weapon_Rifle) {
				playerFire->weapon_Rifle->Ammo += (30);
				result = true;
			}
			break;
		case EItemID::Arrow:
			if (playerFire->weapon_Bow) {
				playerFire->weapon_Bow->currAmmo += (20);
				result = true;
			}
			break;

		case EItemID::StoneWall:	{
			ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->StoneWall)));
			result = true;
		}
			break;
		case EItemID::SendBagWall: {
			ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Sandbag)));
			result = true;
		}
			break;
		case EItemID::ConcreteWall:	{
			ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->BrokenWalll)));
			result = true;
		}
			break;
		case EItemID::Barricade: {
			ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Barricade)));
			result = true;
		}
			break;
		
		case EItemID::Turret: {
			ItemArr.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Turret)));
			result = true;
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

	if (result) {
		Grace -= ItemGrace * ItemCount;
		Mineral -= ItemMineral * ItemCount;
	}
}

void ATPSPlayer::CreateItem(TArray<ABuildableItem*>& items, EItemID itemId, int count)
{	

	for (int i = 0; i < count; i++) {

		switch (itemId)
		{
			// 돌담
		case EItemID::StoneWall: {
			items.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->StoneWall)));
		}
				break;

				// 바리케이드
		case EItemID::Barricade: {
			items.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Barricade)));
		}
				break;

				// 부서진담
		case EItemID::ConcreteWall: {
			items.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->BrokenWalll)));
		}
				break;

				// 모래주머니 담
		case EItemID::SendBagWall: {
			items.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Sandbag)));
		}
				break;

				// 터렛
		case EItemID::Turret: {
			items.Add(Cast<ABuildableItem>(GetWorld()->SpawnActor(itemFactory->Turret)));
		}
				break;
		}
	}

	return;
}

void ATPSPlayer::OnPlayerDie()
{
	playerUI->screenUI->RespawnEvent(true);
	playerUI->screenUI->RespawnTime = respawnTime;
	playerUI->screenUI->RespawnTimeLoop();
	if (heartAudio)
		heartAudio->Stop();

	GetWorldTimerManager().SetTimer(PlayerRespawnTimer, FTimerDelegate::CreateLambda([&]() {
		isDie = false;
		AddHP(maxHp);
		PlayMontage(AM_Spawn);
		SpawnEffect->Activate(true);
		}), respawnTime, false);
}



