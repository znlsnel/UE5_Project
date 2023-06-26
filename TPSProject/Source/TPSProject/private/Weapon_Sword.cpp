// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Sword.h"
#include "Enemy.h"
#include "PlayerUI.h"
#include "PlayerAbilityComp.h"
#include "ScreenUI.h"

#include <particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>

AWeapon_Sword::AWeapon_Sword() : Super()
{
	Tags.Add(TEXT("Sword"));
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weaponmeshcomp"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_SwordB/SK_Blade_SwordB.SK_Blade_SwordB'"));

	if (tempMesh.Succeeded())
	{
		weaponMeshComp->SkeletalMesh = tempMesh.Object;
	}

	RootComponent = weaponMeshComp;

	pickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("pickupCollision"));
	enemySensor = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemySensor"));
	swordEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("swordEffect"));
	ShieldEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShieldEffect"));
	BlockEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlockEffect"));

	pickupCollision->SetRelativeLocation(FVector(0, 0, 40));
	pickupCollision->SetRelativeScale3D(FVector(1, 1, 2.25));
	pickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	pickupCollision->SetupAttachment(RootComponent);
	weaponType = WeaponType::Sword;
	attachCharacterSocketName = FName("RightHandSocket");

	enemySensor->SetupAttachment(weaponMeshComp);
	enemySensor->AttachToComponent(weaponMeshComp, FAttachmentTransformRules::KeepRelativeTransform);

	swordEffect->SetupAttachment(weaponMeshComp);
	swordEffect->AttachToComponent(weaponMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
	PrimaryActorTick.bCanEverTick = true;

	ShieldEffect->SetupAttachment(weaponMeshComp);
	ShieldEffect->AttachToComponent(weaponMeshComp, FAttachmentTransformRules::KeepRelativeTransform);

	BlockEffect->SetupAttachment(weaponMeshComp);
	BlockEffect->AttachToComponent(weaponMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
}

void AWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();
	enemySensor->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Sword::OnOverlapBegin);
	swordEffect->Activate(true);
	ShieldEffect->Deactivate();
	BlockEffect->Deactivate();
}

void AWeapon_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SwordMoveOn) {
		FVector moveoffset = myPlayer->GetActorForwardVector() * 10;
		myPlayer->AddActorWorldOffset(moveoffset);
	}
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
	myPlayer->PlayMontage(CharacterFireAM, attackSection);
	SwordMoveOn = true;
	IsActiveSword = true;

	GetWorldTimerManager().SetTimer(swordMoveTimer, FTimerDelegate::CreateLambda(
		[&]() {
			SwordMoveOn = false;
		}), swordMoveTime, false);
}

void AWeapon_Sword::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy")) == false)
		return;

	if (myPlayer == nullptr) return;

	if (myPlayer->IsPlayingMontage(CharacterFireAM) == false)
		return;

	int finalDamage = weapDamage + myPlayer->abilityComp->GetSkillInfo(SkillType::swordProficiency)->powerValue;

	tempEnemy = Cast<AEnemy>(OtherActor);
	tempEnemy->OnDamage(finalDamage, SweepResult.BoneName, myPlayer);


	int randInt = FMath::RandRange(1, 100);
	bool isDoubleAttack = false;
	if (randInt < myPlayer->abilityComp->GetSkillInfo(SkillType::DoubleAttack)->powerValue)
		isDoubleAttack = true;
	
	if (isDoubleAttack) {
		GetWorldTimerManager().SetTimer(doubleAttackTimer, FTimerDelegate::CreateLambda([&]() {
			if (tempEnemy) {
				int finalDamage = weapDamage + myPlayer->abilityComp->GetSkillInfo(SkillType::swordProficiency)->powerValue;
				tempEnemy->OnDamage(finalDamage, SweepResult.BoneName, myPlayer);
				tempEnemy = nullptr;
			}
			}), 0.1f, false);
	}
}

void AWeapon_Sword::BlockAttack()
{
	if (GetWorld()->GetTimeSeconds() - lastBlockingTime < BlockingCoolTime) {
		return;
	} 
	myPlayer->PlayMontage(AM_Blocking, TEXT("BlockingStart"));
	IsActiveSword = true;

	GetWorldTimerManager().SetTimer(swordMoveTimer, FTimerDelegate::CreateLambda(
		[&]() {
			if (isBlocking == false)
				IsActiveSword = false;
		}), 0.5f, false);
}

void AWeapon_Sword::OnBlocking(bool On)
{
	isBlocking = On;
	if (isBlocking) {
		ShieldEffect->Activate(true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShieldSound, GetActorLocation());
		UScreenUI* tempUI = myPlayer->playerUI->screenUI;
		tempUI->ShieldTime = BlockingCoolTime;
		tempUI->ShieldTimeText = FString::Printf(TEXT("%d"), (int)BlockingCoolTime);
		lastBlockingTime = GetWorld()->GetTimeSeconds();

		if (GetWorld()->GetTimerManager().IsTimerActive(myPlayer->abilityComp->skillTimerHandle) == false)
			myPlayer->abilityComp->OperateSkillTimer();
	}
	else {
		ShieldEffect->Deactivate();
		IsActiveSword = false;
	}
}

