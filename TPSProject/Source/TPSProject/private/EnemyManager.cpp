// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "RoundUI.h"

#include <EngineUtils.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include <UObject/CoreNet.h>
#include "Containers/ContainerAllocationPolicies.h"
#include <GameFramework/Actor.h>


// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();


	GetWorld()->GetTimerManager().SetTimer(startTimerHandle, this, &AEnemyManager::StartGame, 5.5f);
}

void AEnemyManager::StartGame_Implementation()
{
	roundUI = CreateWidget<URoundUI>(GetWorld(), roundUIFactory);
	// 1. ������ ���� �ð� ���ϱ�
	if (IsValid(roundUI)) roundUI->AddToViewport();

	roundUI->roundTime = 10;

	FindSpawnPoints();
	StartRound();
}



// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AEnemyManager::CreateEnemy_Implementation()
{
	// ���� ��ġ ���ϱ�
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);

	if (enemyPool.Num() >= monsterSpawnLimit)
	{
		for (auto monster : enemyPool)
		{
			if (IsValid(monster) == false) return;

			if (monster->fsm->isActive == false)
			{
				monster->fsm->InitializeEnemy(spawnPoints[index]->GetActorLocation());
				monster->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp);
				break;
			}
		}
	}
	else
	{
		AEnemy* enemy;
		for (int i = 0; i < 10; i++)
		{
			FVector tempPos = spawnPoints[index]->GetActorLocation();
			tempPos.Z += 150;

			enemy = Cast<AEnemy>(GetWorld()->SpawnActor<AActor>(enemyFactory, tempPos, FRotator(0, 0, 0)));




			if (IsValid(enemy))
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Yeeeeeeeeeeeeeeeeeeee"));
				break;
			}
			else
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("faild CreateEnemy"));
				UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Location : %f, %f, %f"), spawnPoints[index]->GetActorLocation().X, spawnPoints[index]->GetActorLocation().Y, spawnPoints[index]->GetActorLocation().Z));

			}
		}


		if (enemy)
		{
			enemy->fsm->InitializeEnemy(spawnPoints[index]->GetActorLocation());
			enemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp);
			enemyPool.Add(enemy);
			enemy->enemyManager = this;
		}
	}


	// �ٽ� ���� �ð��� CreateEnemy �Լ��� ȣ��ǵ��� Ÿ�̸� ����
	float createTime = FMath::RandRange(minTime, maxTime);

	// �ٽ� ȣ��ǰԲ� Ÿ�̸Ӹ� ������
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);

}

void AEnemyManager::FindSpawnPoints()
{
	// �˻����� ã�� ����� ������ �迭
	TArray<AActor*> allActors;
	
	// ���ϴ� Ÿ���� ���� ��� ã�ƿ���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);

	for (auto spawn : allActors)
	{
		// ã�� ������ �̸��� �ش� ���ڿ��� �����ϰ� �ִٸ�
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			// ���� ��Ͽ� �߰�
			spawnPoints.Add(spawn);
		}
	}
}

void AEnemyManager::StartRound()
{
	roundUI->roundTime--;
	if (roundUI->roundTime == 0)
	{
		roundUI->isEndBreakTime = isbreakTime;
		if (isbreakTime) // ���½ð� ����
		{
			roundUI->Round++;
			isbreakTime = false;
			roundUI->roundTime = 30;

			enemyBonusAttackPower = (roundUI->Round - 1) * 5;
			enemyBonusHp *= 1.1f;

			float createTime = FMath::RandRange(minTime, maxTime);
			GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
		}
		else // ���½ð� ����
		{
			isbreakTime = true;
			roundUI->roundTime = 15;

			GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandle);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(roundTimerHandle, this, &AEnemyManager::StartRound, 1.0f);
}

void AEnemyManager::GetLifeTimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyManager, enemyPool);
}


