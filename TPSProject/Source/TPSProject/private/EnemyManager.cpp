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

	roundUI = CreateWidget<URoundUI>(GetWorld(), roundUIFactory);
	// 1. 랜덤한 생성 시간 구하기
	GetWorld()->GetTimerManager().SetTimer(startTimerHandle, this, &AEnemyManager::ATVUI, 2.5f);


	FindSpawnPoints();
	StartRound();
}

void AEnemyManager::ATVUI()
{
	if (IsValid(roundUI)) roundUI->AddToViewport();
}



// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemyManager::SpawnEnemy_Implementation()
{


	int spawnIndex = FMath::RandRange(0, spawnPoints.Num() - 1);

	if (enemyPool.Num() >= monsterSpawnLimit)
	{
		for (auto monster : enemyPool)
		{
			if (IsValid(monster) == false) return;

			if (monster->fsm->isActive == false)
			{
				FVector genPos = spawnPoints[spawnIndex]->GetActorLocation();
				genPos.Z += 150;

				RecycleEnemy(monster, genPos);
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

void AEnemyManager::CreateEnemy_Implementation(FVector location)
{
	AEnemy* enemy = Cast<AEnemy>(GetWorld()->SpawnActor<AActor>(enemyFactory, location, FRotator(0, 0, 0)));

	if (IsValid(enemy))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Yeeeeeeeeeeeeeeeeeeee"));
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Location : %f, %f, %f"), location.X, location.Y, location.Z));

		enemy->fsm->InitializeEnemy(location);
		enemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp);
		enemyPool.Add(enemy);
		enemy->enemyManager = this;
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("faild CreateEnemy"));
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Location : %f, %f, %f"), location.X, location.Y, location.Z));

	}
}

void AEnemyManager::RecycleEnemy_Implementation(AEnemy* enemy, FVector location)
{
	enemy->fsm->InitializeEnemy(location);
	enemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp);
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

void AEnemyManager::StartRound_Implementation()
{
	if (GetNetMode() != NM_DedicatedServer) return;

	serverTime--;
	SyncTime(serverTime);
	if (serverTime == 0)
	{
		if (isbreakTime) // 쉬는시간 종료
		{
			serverRound++;
			isbreakTime = false;
			serverTime = 30;

			enemyBonusAttackPower = (serverRound - 1) * 5;
			enemyBonusHp *= 1.5f;

			float createTime = FMath::RandRange(minTime, maxTime);
			GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::SpawnEnemy, createTime);
		}
		else // 쉬는시간 시작
		{
			isbreakTime = true;
			serverTime = 15;
			GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandle);
		}
	}

	GetWorldTimerManager().SetTimer(roundTimerHandle, this, &AEnemyManager::StartRound, 1.0f);

}

void AEnemyManager::SyncTime_Implementation(int currTime)
{
	if (GetNetMode() == NM_DedicatedServer)
		return;
	
	roundUI->roundTime = currTime ;

	if (currTime == 0)
	{
		roundUI->isEndBreakTime = isbreakTime;
		if (isbreakTime) // 쉬는시간 종료
		{
			roundUI->Round = serverRound;
			isbreakTime = false;
			roundUI->roundTime = 30;
		}
		else // 쉬는시간 시작
		{
			isbreakTime = true;
			roundUI->roundTime = 15;
		}
	}

}

void AEnemyManager::IncreaseKillCount_Implementation()
{
	
	if (GetNetMode() == NM_DedicatedServer)
		return;
	

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("IncreaseKillCount!"));
	roundUI->UpdateKillCount();
}



void AEnemyManager::GetLifeTimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyManager, enemyPool);
	DOREPLIFETIME(AEnemyManager, roundUI);
	DOREPLIFETIME(AEnemyManager, spawnPoints);
	//DOREPLIFETIME(AEnemyManager, roundTimerHandle);
	//DOREPLIFETIME(AEnemyManager, spawnTimerHandle);
	//DOREPLIFETIME(AEnemyManager, startTimerHandle);
}


