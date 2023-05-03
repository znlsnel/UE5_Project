// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyManager.h"
#include "DamageUI.h"
#include "DamageUIActor.h"
#include "RoundUI.h"

// Sets default values
AEnemy::AEnemy()
{
	bReplicates = true;
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


	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	while (damageActorArr.Num() < DActorCount)
	{
		ADamageUIActor* tempUIActor = GetWorld()->SpawnActor<ADamageUIActor>(damageActorFactory);

		if (tempUIActor)
			damageActorArr.Add(tempUIActor);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AddWorldDamageUI(FRotator genRot, int Damage)
{
	FVector genPos = GetActorLocation();
	genPos.Z += 120.f;

	if (DActorindex >= 5) DActorindex = 0;

	damageActorArr[DActorindex]->InitializeDamageActor(genPos, genRot, Damage);

	DActorindex++;
}

void AEnemy::DieEvent()
{
	enemyManager->roundUI->UpdateKillCount();
}
