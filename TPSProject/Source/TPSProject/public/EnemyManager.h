// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"


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


	//UFUNCTION(client, Reliable)
		void StartGame();
	//	void StartGame_Implementation();
		void ATVUI();
	// Called every frame

	virtual void Tick(float DeltaTime) override;

	class ATPSPlayer* locallyPlayer;
	
		void SpawnEnemy();
		void CreateEnemy( FVector location);
		void AddEnemy(FVector location);
	//UFUNCTION(Client, Reliable)
		void FindSpawnPoints();
		//void FindSpawnPoints_Implementation();


	UFUNCTION(BlueprintCallable)
		void RoundEvent(bool start);
	
		//void LoopCount();

public:
	// ������ ���� �˶� Ÿ�̸�
	FTimerHandle spawnTimerHandle;
	FTimerHandle SpawnWaitTimer;
	bool isbreakTime = true;
	int currRound = 0;

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
	AEnemy* tempEnemy;
	int32 monsterSpawnLimit = 15;

	float enemyBonusAttackPower = 0.f;
	float enemyBonusHp = 1.f;
};
