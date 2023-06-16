// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyManager.h"
#include "DamageUI.h"
#include "DamageUIActor.h"
#include "RoundUI.h"
#include "TPSPlayer.h"
#include "EnemyHpBar.h"

#include <Components/WidgetComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>

// Sets default values
AEnemy::AEnemy()
{
	//bReplicates = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Enemy"));

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// 1. 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/ParagonRiktor/Characters/Heroes/Riktor/Meshes/Riktor.Riktor'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeScale3D(FVector(0.88));
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	// 월드에 배치되거나 스폰될 때 자동으로
	// AIController부터 Possess될 수 있도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	myController = GetController();

	HpBarWgComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBarWgComp->SetupAttachment(GetMesh());
	HpBarWgComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < DActorCount; i++)
	{
		ADamageUIActor* tempUIActor = GetWorld()->SpawnActor<ADamageUIActor>(damageActorFactory);

		if (tempUIActor)
		{
			damageActorArr.Add(tempUIActor);
			tempUIActor->myPlayer = player;
		}
	}


	HpBar = CreateWidget<UEnemyHpBar>(GetWorld(), HpBarWg);
	HpBar->enemyFsm = fsm;
	HpBarWgComp->SetWidget(HpBar);

}
#include <Kismet/KismetMathLibrary.h>
// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fsm == nullptr)
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("no fsm"));

	if (fsm && fsm->isActive && player)
	{
		HpBarWgComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation()));
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AddWorldDamageUI(FRotator genRot, int Damage)
{

	FVector genPos = GetActorLocation();
	FRotator GenRotate = genRot;
	genPos.Z = HpBarWgComp->GetComponentLocation().Z + 20.f;

	if (DActorindex >= 5) DActorindex = 0;

	if (player)
	{
		FRotator a = player->GetActorRotation();
		a.Yaw *= -1;
		a.Pitch = 0;
		GenRotate = a;
	}

	ATPSPlayer* tempPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	damageActorArr[DActorindex]->InitializeDamageActor(genPos, GenRotate, Damage, tempPlayer);

	DActorindex++;
}




void AEnemy::DieEvent(ATPSPlayer* AttackPlayer)
{

}

void AEnemy::OnDamage(int damage, ATPSPlayer* AttackPlayer)
{
	fsm->OnDamageProcess(damage, AttackPlayer);
}

void AEnemy::SetTarget(AActor* target)
{
	fsm->SetTarget(target);
}

bool AEnemy::isActive()
{
	return fsm->isActive;
}

