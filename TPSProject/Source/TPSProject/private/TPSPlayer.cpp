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

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include <Blueprint/UserWidget.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/SkeletalMeshComponent.h>
#include <Blueprint/UserWidget.h>



// Sets default values
ATPSPlayer::ATPSPlayer()
{

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

		// RootComponent �� ���������� ĸ�� �ݸ��� ������Ʈ�� �ǹ���
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

}

ATPSPlayer::~ATPSPlayer()
{
	ATPSPlayer::playerId = 0;

}



void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	hp = initialHp;
	if(playerUI->screenUI)
		playerUI->screenUI->UpdateScreenUI();

	playerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	tickUpdateFunctions.Broadcast();
	
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	onInputBindingDelegate.Broadcast(PlayerInputComponent);

}

void ATPSPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
}

void ATPSPlayer::PlayMontageInServer_Implementation(UAnimMontage* AM)
{
	MulticastAnimMontage(AM);
}

void ATPSPlayer::MulticastAnimMontage_Implementation(UAnimMontage* AM)
{
	GetMesh()->GetAnimInstance()->Montage_Play(AM);
}

void ATPSPlayer::createNiagara_Implementation(FHitResult pHitResult)
{
	MulticastNiaga(pHitResult);
}

void ATPSPlayer::MulticastNiaga_Implementation(FHitResult pHitResult)
{
	playerFire->GetWeapon()-> createNiagara(pHitResult);
}

void ATPSPlayer::DoubleClickInServer_Implementation(DashType dashType)
{
	DoubleClickMulticast(dashType);
}

void ATPSPlayer::DoubleClickMulticast_Implementation(DashType dashType)
{
	Cast<UPlayerMove>(playerMove)->Dash(dashType);
}

void ATPSPlayer::AddItemInServer_Implementation(AItem* item)
{
	AddItemMulticast(item);
}

void ATPSPlayer::AddItemMulticast_Implementation(AItem* item)
{
	item->myPlayer = this;
	GetInventory()->AddItemToInventory(item);
	//item->SetActorHiddenInGame(true);
}

void ATPSPlayer::DropItemInServer_Implementation(class AItem* item)
{
	DropItemMulticast(item);
}

void ATPSPlayer::DropItemMulticast_Implementation(class AItem* item)
{
	item->DropItemOnGround();
}

void ATPSPlayer::SyncWeaponInServer_Implementation(AWeapon* weapon)
{
	SyncWeaponMulticast(weapon);
}

void ATPSPlayer::SyncWeaponMulticast_Implementation(AWeapon* weapon)
{
	weapon->SynchronizeWhitPlayer(this);
}

void ATPSPlayer::ClickWidget_Implementation(bool isFire)
{
	//myController->ClickBPWidget(isFire);

	ClickWidgetMulti(isFire);
	//playerFire->GetWeapon()->ClickWorldWidget(isFire);
}

void ATPSPlayer::ClickWidgetMulti_Implementation(bool isFire)
{
	//playerFire->GetWeapon()->ClickWorldWidget(isFire);
	ClickBPWidget(isFire);
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





UInventory* ATPSPlayer::GetInventory()
{
	return playerUI->screenUI->inventory;
}

void ATPSPlayer::GetMoney(int money)
{
	myController->storeActor->storeUI->Money += money;
}







void ATPSPlayer::OnHitEvent_Implementation(int damage)
{

	OnDamage(damage);
}

void ATPSPlayer::OnDamage_Implementation(int damage)
{
	OnDamageMulti(damage);
}

void ATPSPlayer::OnDamageMulti_Implementation(int damage)
{
	if (hp <= 0) return;
	hp -= damage;
	if (playerUI->screenUI)
		playerUI->screenUI->UpdateScreenUI();

	if (hp <= 0)
	{
		hp = 0;
		
		if (IsLocallyControlled())
			OnGameOver();
		isDie = true;

	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);


}



