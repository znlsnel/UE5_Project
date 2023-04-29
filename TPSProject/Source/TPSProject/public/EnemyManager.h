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
	// ������ ���� �˶� Ÿ�̸�
	FTimerHandle spawnTimerHandle;
	FTimerHandle roundTimerHandle;
	FTimerHandle startTimerHandle;
	bool isbreakTime = true;

	// ���� �ð� ���� �ּڰ�
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float minTime = 3.f;

	// ���� �ð� ���� �ִ�
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float maxTime = 5.5f;

	// ������ ��ġ ���� �迭
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor*> spawnPoints;

	// AEnemy Ÿ���� �������Ʈ �Ҵ���� ����
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
