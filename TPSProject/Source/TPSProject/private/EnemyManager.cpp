// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include "Monster.h"
#include "EnemyFSM.h"
#include "RoundUI.h"
#include "TPSPlayer.h"


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
	SetReplicates(true);
	
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();


	StartGame();
}

void AEnemyManager::StartGame()
{

	TArray<class AActor*, FDefaultAllocator> playerArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATPSPlayer::StaticClass(), playerArr);

	for (auto p : playerArr)
	{
		if (Cast<ATPSPlayer>(p)->isMyPlayer == true)
		{
			locallyPlayer = Cast<ATPSPlayer>(p);
		}
	}

	FindSpawnPoints();
}



// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemyManager::SpawnEnemy()
{

	int spawnIndex = FMath::RandRange(0, spawnPoints.Num() - 1);
	if (enemyPool.Num() >= monsterSpawnLimit)
	{
		for (auto enemy : enemyPool)
		{
			if (IsValid(enemy) == false) return;

			if (enemy->fsm->isActive == false)
			{
				FVector genPos = spawnPoints[spawnIndex]->GetActorLocation();
				genPos.Z += 150;

				enemy->fsm->InitializeEnemy(genPos);
				enemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp);

				break;
			}
		}
	}
	else
	{

		FVector tempPos = spawnPoints[spawnIndex]->GetActorLocation();
		tempPos.Z += 150;

		CreateEnemy(tempPos);

	}
	// 다시 랜덤 시간에 CreateEnemy 함수가 호출되도록 타이머 설정
	float createTime = FMath::RandRange(minTime, maxTime);

	// 다시 호출되게끔 타이머를 설정함
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::SpawnEnemy, createTime);
}

void AEnemyManager::CreateEnemy(FVector location)
{


	AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(enemyFactory, location, FRotator(0, 0, 0));

	if (IsValid(enemy))
	{
		if (locallyPlayer)
			enemy->locallyPlayer = this->locallyPlayer;

		if (IsValid(enemy->fsm) == false) {
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No fsm"));
			return;
			
		}

		enemy->fsm->InitializeEnemy(location);
		enemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp);
		enemyPool.Add(enemy);
		enemy->enemyManager = this;
	}
}


void AEnemyManager::FindSpawnPoints()
{
	
	// 검색으로 찾은 결과를 저장할 배열
	TArray<AActor*> allActors;
	
	// 원하는 타입의 액터 모두 찾아오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);

	for (auto spawn : allActors)
	{
		// 찾은 액터의 이름에 해당 문자열을 포함하고 있다면
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			// 스폰 목록에 추가
			spawnPoints.Add(spawn);
		}
	}
}


void AEnemyManager::RoundEvent(bool start)
{

	if (start)
	{
		currRound++;
		isbreakTime = false;

		enemyBonusAttackPower = (currRound - 1) * 5;
		enemyBonusHp *= 1.5f;

		float createTime = FMath::RandRange(minTime, maxTime);
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::SpawnEnemy, createTime);
	}
	else
	{
		isbreakTime = true;
		GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandle);
	}
}



