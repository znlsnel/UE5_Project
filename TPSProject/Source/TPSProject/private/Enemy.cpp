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

	// 1. 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/ParagonRiktor/Characters/Heroes/Riktor/Meshes/Riktor.Riktor'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeScale3D(FVector(0.88));
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// 월드에 배치되거나 스폰될 때 자동으로
	// AIController부터 Possess될 수 있도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	myController = GetController();








	HpBarWgComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	while (damageActorArr.Num() < DActorCount)
	{
		ADamageUIActor* tempUIActor = GetWorld()->SpawnActor<ADamageUIActor>(damageActorFactory);

		if (tempUIActor)
		{
			damageActorArr.Add(tempUIActor);
			tempUIActor->myPlayer = locallyPlayer;
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
	if (fsm && fsm->isActive && locallyPlayer)
	{
		HpBarWgComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), locallyPlayer->GetActorLocation()));
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AddWorldDamageUI_Implementation(FRotator genRot, int Damage)
{

	AddWorldDamageUI_M(genRot, Damage);
}

void AEnemy::AddWorldDamageUI_M_Implementation(FRotator genRot, int Damage)
{
	FVector genPos = GetActorLocation();
	FRotator GenRotate = genRot;
	genPos.Z += 120.f;

	if (DActorindex >= 5) DActorindex = 0;

	if (locallyPlayer)
	{
		FRotator a = locallyPlayer->GetActorRotation();
		a.Yaw *= -1;
		a.Pitch = 0;
		GenRotate = a;
	}

	ATPSPlayer* tempPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	damageActorArr[DActorindex]->InitializeDamageActor(genPos, GenRotate, Damage, tempPlayer);

	DActorindex++;
}

void AEnemy::DieEvent(ATPSPlayer* player)
{

}

void AEnemy::OnDamage(int damage)
{
}

