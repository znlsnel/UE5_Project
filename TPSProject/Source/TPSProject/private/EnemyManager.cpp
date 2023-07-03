// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include "Monster.h"
#include "EnemyFSM.h"
#include "RoundUI.h"
#include "TPSPlayer.h"

#include <Kismet/KismetMathLibrary.h>
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


void AEnemyManager::SpawnEnemy()
{
	int spawnIndex = GetSpawnIndex();

	if (EnemyPool.Num() >= spawnLimit) {

		for (auto enemy : EnemyPool)
			if (RecycleEnemy(enemy, spawnIndex))
				break;
	}
	else
	{
		FVector tempPos = spawnPoints[spawnIndex]->GetActorLocation();
		tempPos.Z += 150;

		bool spawnParasite = FMath::RandRange(0, 1) == 0;
		if (spawnParasite)
			CreateEnemy(tempPos, Parasite, EnemyPool);
		else
			CreateEnemy(tempPos, Yaku, EnemyPool);
	}

	// 다시 랜덤 시간에 CreateEnemy 함수가 호출되도록 타이머 설정
	float createTime = FMath::RandRange(MonsterSpawnMinTime, MonsterSpawnMaxTime);

	// 다시 호출되게끔 타이머를 설정함
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, FTimerDelegate::CreateLambda([&]() {
		SpawnEnemy();
		}), createTime, false);
}

void AEnemyManager::SpawnBossMonster( )
{
	int spawnIndex = GetSpawnIndex();

	TArray<AEnemy*>* tempArr = &KwangPool;
	if (spawnRiktor) {
		tempArr = &RiktorPool;
	}

	if (tempArr->Num() >= spawnLimit) {
		for (auto boss : *tempArr)
			if (RecycleEnemy(boss, spawnIndex))
				break;
	}
	else{
		FVector tempPos = spawnPoints[spawnIndex]->GetActorLocation();
		tempPos.Z += 150;

		if (spawnRiktor)
			CreateEnemy(tempPos, RiktorFactory, RiktorPool);
		else
			CreateEnemy(tempPos, KwangFactory, KwangPool);
	}



	spawnRiktor = spawnRiktor ? false : true;


	float randTime =  FMath::FRandRange(BossSpawnMinTime, BossSpawnMaxTime);
	GetWorld()->GetTimerManager().SetTimer(bossSpawnTimerHandle, this, &AEnemyManager::SpawnBossMonster, randTime, false);
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

void AEnemyManager::CreateEnemy(FVector location, TSubclassOf<class AEnemy>& enemyFactory, TArray<class AEnemy*>& pool)
{

	tempEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(enemyFactory));

	if (tempEnemy)
		tempEnemy->SetActorHiddenInGame(true);
	else
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Enemy"));

	AddEnemy(location, pool);

	//GetWorld()->GetTimerManager().SetTimer(SpawnWaitTimer, FTimerDelegate::CreateLambda(
	//	[&, location]() {
	//		AddEnemy(location);
	//	}
	//), 1.f, false);
}

void AEnemyManager::AddEnemy(FVector location, TArray<AEnemy*>& pool)
{
	if (IsValid(tempEnemy))
	{
		if (player == nullptr)
			player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (player)
			tempEnemy->player = player;

		if (tempEnemy->fsm == nullptr) {
			return;
		}


		tempEnemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp, currRound);
		tempEnemy->fsm->InitializeEnemy(location);


		pool.Add(tempEnemy);

		tempEnemy->enemyManager = this;
	}
}

bool AEnemyManager::RecycleEnemy(AEnemy* enemy, int SpawnIndex)
{
	if (IsValid(enemy) == false || enemy->fsm->isActive) 
		return false;

	FVector genPos = spawnPoints[SpawnIndex]->GetActorLocation();
	genPos.Z += 150;

	enemy->fsm->RoundInitEnemy(enemyBonusAttackPower, enemyBonusHp, currRound);
	enemy->fsm->InitializeEnemy(genPos);

	return true;
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
			enemyBonusAttackPower += 1;
			enemyBonusHp += 10;
		}

		currRound++;
		// All Enemy Die

		SpawnEnemy();
		SpawnBossMonster();
		//float createTime = FMath::RandRange(MonsterSpawnMinTime, MonsterSpawnMaxTime);
		//GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, FTimerDelegate::CreateLambda([&]() {
		//	SpawnEnemy();
		//	}), createTime, false);
	}
	else
	{
		isbreakTime = true;
		GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandle);

		GetWorld()->GetTimerManager().ClearTimer(bossSpawnTimerHandle);

		for (auto enemy : EnemyPool)
			enemy->OnDamage(enemy->fsm->maxHp * 100,"",nullptr,true);
		for (auto enemy : RiktorPool)
			enemy->OnDamage(enemy->fsm->maxHp * 100, "", nullptr, true);
		for (auto enemy : KwangPool)
			enemy->OnDamage(enemy->fsm->maxHp * 100, "", nullptr, true);

	}
}



