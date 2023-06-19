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

	player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


	FindSpawnPoints();
}



// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemyManager::SpawnEnemy(bool SpawnBoss)
{
	int spawnIndex = GetSpawnIndex();
	int spawnLimit = 0;

	EnemyPoolType poolType = EnemyPoolType::Common;
	if (SpawnBoss)
		poolType = EnemyPoolType::Boss;
	else {
		int a = FMath::RandRange(0, 15);
		if (a > 12)
			poolType = EnemyPoolType::Special;
	}

	TArray<class AEnemy*>* tempPool;
	if (poolType == EnemyPoolType::Common) {
		tempPool = &CommonEnemyPool;
		spawnLimit = commonSpawnLimit;
	}
	else if (poolType == EnemyPoolType::Special) {
		tempPool = &SpecialEnemyPool;
		spawnLimit = SpecialSpawnLimit;
	}
	else {
		tempPool = &BossEnemyPool;
		spawnLimit = BossSpawnLimit;
	}



	if (tempPool->Num() >= commonSpawnLimit) {
		for (auto enemy : *tempPool)
			if (RecycleEnemy(enemy, spawnIndex))
				break;
	}

	else
	{
		FVector tempPos = spawnPoints[spawnIndex]->GetActorLocation();
		tempPos.Z += 150;

		CreateEnemy(tempPos, poolType);
	}
	// 다시 랜덤 시간에 CreateEnemy 함수가 호출되도록 타이머 설정
	float createTime = FMath::RandRange(minTime, maxTime);

	// 다시 호출되게끔 타이머를 설정함
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, FTimerDelegate::CreateLambda([&]() {
		SpawnEnemy();
		}), createTime, false);
}

int AEnemyManager::GetSpawnIndex()
{
	int result = FMath::RandRange(0, spawnPoints.Num() - 1);

	bool isFind = false;

	bool find = false;
	for (auto i : preSpawnIndex) {
		if (i == result) {
			find = true;
			break;
		}
	}
	
	if (find)
		return GetSpawnIndex();

	else if (preSpawnIndex.Num() >= 3) {
		for (int i = preSpawnIndex.Num() - 1; i > 0; i--)
		{
			preSpawnIndex[i] = preSpawnIndex[i - 1];
		}
		preSpawnIndex[0] = result;
	}
	else
		preSpawnIndex.Add(result);

	return result;
}

void AEnemyManager::CreateEnemy(FVector location, EnemyPoolType poolType)
{

	switch (poolType) {
	case EnemyPoolType::Common: {
		int a = FMath::RandRange(0, 5);
		if (a < 3)
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(RiktorFactory));
		else if (a < 4)
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(SuperFactory));
		else 
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(FastRiktorFactory));
	}
		break;
	case EnemyPoolType::Special: {
		int a = FMath::RandRange(0, 5);
		if (a < 2)
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(BigRiktorFactory));
		else if (a < 4)
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(HelixFactory));
		else
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(BomberFactory));
	}
		break;

	case EnemyPoolType::Boss: {
		int a = FMath::RandRange(0, 2);
		if (a < 1)
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(GiantRiktorFactory));
		else
			tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(KrakenFactory));
	}
		break;
	}

	if (tempEnemy)
		tempEnemy->SetActorHiddenInGame(true);
	else
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Enemy"));

	GetWorld()->GetTimerManager().SetTimer(SpawnWaitTimer, FTimerDelegate::CreateLambda(
		[&, location]() {
			AddEnemy(location, poolType);
		}
	), 1.f, false);
}

void AEnemyManager::AddEnemy(FVector location, EnemyPoolType poolType)
{
	if (IsValid(tempEnemy))
	{
		if (player == nullptr)
			player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (player)
			tempEnemy->player = player;

		if (tempEnemy->fsm == nullptr) {
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No FSM"));
			return;
		}


		tempEnemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp, currRound);
		tempEnemy->fsm->InitializeEnemy(location);

		switch (poolType)
		{
		case EnemyPoolType::Common:
			CommonEnemyPool.Add(tempEnemy);
			break;

		case EnemyPoolType::Special:
			SpecialEnemyPool.Add(tempEnemy);
			break;

		case EnemyPoolType::Boss:
			BossEnemyPool.Add(tempEnemy);
			break;
		}


		tempEnemy->enemyManager = this;
	}
}

bool AEnemyManager::RecycleEnemy(AEnemy* enemy, int SpawnIndex)
{
	if (IsValid(enemy) == false) return false;

	if (enemy->fsm->isActive == false)
	{
		FVector genPos = spawnPoints[SpawnIndex]->GetActorLocation();
		genPos.Z += 150;

		enemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp, currRound);
		enemy->fsm->InitializeEnemy(genPos);

		return true;
	}
	return false;
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
		isbreakTime = false;

		if (currRound > 0) {
			enemyBonusAttackPower += 3;
			enemyBonusHp += 3;
		}

		currRound++;
		// All Enemy Die
		SpawnEnemy(true);
		float createTime = FMath::RandRange(minTime, maxTime);
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, FTimerDelegate::CreateLambda([&]() {
			SpawnEnemy();
			}), createTime, false);
	}
	else
	{
		isbreakTime = true;
		GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandle);

		for (auto enemy : CommonEnemyPool)
			enemy->OnDamage(enemy->fsm->maxHp * 100, "");
		for (auto enemy : SpecialEnemyPool)
			enemy->OnDamage(enemy->fsm->maxHp * 100, "");
		for (auto enemy : BossEnemyPool)
			enemy->OnDamage(enemy->fsm->maxHp * 100, "");
	}
}



