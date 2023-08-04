// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Sword.h"
#include "Enemy.h"
#include "PlayerUI.h"
#include "PlayerAbilityComp.h"
#include "ScreenUI.h"
#include "EnemyFSM.h"

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
		currCombo = 1;
		myPlayer->PlayMontage(CharacterFireAM, "Attack_Air");
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - lastAttackTime < nextComboDelay){
			++currCombo;
		}
		else{
			currCombo = 1;
		}

		lastAttackTime = GetWorld()->GetTimeSeconds();
		if (currCombo > 4)
			currCombo = 1;

		FName sect = FName(FString::Printf(TEXT("Attack_%d"), currCombo));
		myPlayer->PlayMontage(CharacterFireAM, sect);
	}


	TArray<AActor*> overlappingAcotrs;
	myPlayer->meleeAttackSensor->GetOverlappingActors(overlappingAcotrs);

	isEnemyInSensor = false;
	for (auto actor : overlappingAcotrs) {
		if (actor->GetClass()->IsChildOf(AEnemy::StaticClass())) {
			isEnemyInSensor = true;
			break;
		}
	}

	myPlayer->SetUnableMove(0.65f);
	myPlayer->SetUnableRotate(0.3f);

	if (isEnemyInSensor == false) {
		SwordMoveOn = true;
	}
	
	GetWorldTimerManager().ClearTimer(swordMoveTimer);
	GetWorldTimerManager().SetTimer(swordMoveTimer, FTimerDelegate::CreateLambda(
		[&]() {
			SwordMoveOn = false;
		}), swordMoveTime, false);

}


void AWeapon_Sword::AttackEvent()
{
	if (myPlayer == nullptr) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Player"));
		return;
	}

	for (auto tempActor : overlappingActors) {
		if (tempActor->GetClass()->IsChildOf(AEnemy::StaticClass()) == false) {
			continue;
		}

		int finalDamage = weapDamage + myPlayer->abilityComp->GetSkillInfo(SkillType::swordProficiency)->powerValue;

		AEnemy* enemy = Cast<AEnemy>(tempActor);
		enemy->OnDamage(finalDamage, "", myPlayer);
		
		int randInt = FMath::RandRange(0, 100);
		if (randInt < myPlayer->abilityComp->GetSkillInfo(SkillType::DoubleAttack)->powerValue) {
			tempEnemys.Add(enemy);
		}
	}

	if (tempEnemys.IsEmpty() == false) {
		GetWorldTimerManager().SetTimer(doubleAttackTimer, FTimerDelegate::CreateLambda([&]() {
			for (auto enemy : tempEnemys) {
				int finalDamage = weapDamage + myPlayer->abilityComp->GetSkillInfo(SkillType::swordProficiency)->powerValue;
				enemy->OnDamage(finalDamage, "", myPlayer);
			}
			tempEnemys.Empty();
			}), 0.1f, false);
	}
}

void AWeapon_Sword::BlockAttack()
{
	if (GetWorld()->GetTimeSeconds() - lastBlockingTime < BlockingCoolTime || SwordMoveOn) {
		return;
	} 
	myPlayer->PlayMontage(AM_Blocking, TEXT("BlockingStart"));
}

void AWeapon_Sword::OnBlocking(bool On)
{
	if (On) {
		myPlayer->SetUnableMove(0.65f);
		myPlayer->SetUnableRotate(0.5f);

	}
	isBlocking = On;

	GetWorld()->GetTimerManager().ClearTimer(blockingTimer);
	GetWorld()->GetTimerManager().SetTimer(blockingTimer, FTimerDelegate::CreateLambda(
		[&]() {
			if (isBlocking) {
				isBlocking = false;
			}
		}), 1.f, false);


	if (isBlocking) {
		ShieldEffect->Activate(true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShieldSound, GetActorLocation());
		UScreenUI* tempUI = myPlayer->playerUI->screenUI;
		tempUI->BasicSkillCoolTime(false, BlockingCoolTime);

		lastBlockingTime = GetWorld()->GetTimeSeconds();
		if (GetWorld()->GetTimerManager().IsTimerActive(myPlayer->abilityComp->skillTimerHandle) == false)
			myPlayer->abilityComp->OperateSkillTimer();
	}
	else {
		ShieldEffect->Deactivate();
	}
}

void AWeapon_Sword::ReflectAttack(AEnemy* enemy)
{
	float rand = FMath::FRandRange(0.35f, 0.55f);
	int damage = (int)((float)(enemy->fsm->maxHp) * rand);

	enemy->OnDamage(damage, "", myPlayer);
	BlockEffect->Activate(true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockSound, GetActorLocation());
}

