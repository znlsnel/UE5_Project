// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	void StartGame();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateEnemy();
	void FindSpawnPoints();
	void StartRound();

public:
	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;
	FTimerHandle roundTimerHandle;
	FTimerHandle startTimerHandle;
	bool isbreakTime = true;

	// 랜덤 시간 간격 최솟값
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float minTime = 3.f;

	// 랜덤 시간 간격 최댓값
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float maxTime = 5.5f;

	// 스폰할 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor*> spawnPoints;

	// AEnemy 타입의 블루프린트 할당받을 변수
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> enemyFactory;
	TArray<class AEnemy*> enemyPool;

	UPROPERTY(EditAnywhere, Category = RoundUI)
		TSubclassOf<class URoundUI> roundUIFactory;
	class URoundUI* roundUI;

	int32 monsterSpawnLimit = 15;

	float enemyBonusAttackPower = 0.f;
	float enemyBonusHp = 1.f;
};
