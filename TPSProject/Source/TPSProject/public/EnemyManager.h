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
	virtual void GetLifeTimeReplicatedProps(TArray< class FLifetimeProperty>& OutLifetimeProps) const;

	//UFUNCTION(client, Reliable)
		void StartGame();
	//	void StartGame_Implementation();
		void ATVUI();
	// Called every frame

	virtual void Tick(float DeltaTime) override;



	UFUNCTION(Server, Reliable)
		void SpawnEnemy();
		void SpawnEnemy_Implementation();

	UFUNCTION(client, Reliable)
		void CreateEnemy(FVector location);
		void CreateEnemy_Implementation(FVector location);

		UFUNCTION(client, Reliable)
			void RecycleEnemy(AEnemy* enemy, FVector location);
			void RecycleEnemy_Implementation(AEnemy* enemy, FVector location);

	//UFUNCTION(Client, Reliable)
		void FindSpawnPoints();
		//void FindSpawnPoints_Implementation();

	UFUNCTION(Server, Reliable)
		void StartRound();
		void StartRound_Implementation();
	
		UFUNCTION(NetMulticast, Reliable)
			void SyncTime(int currTime);
			void SyncTime_Implementation(int currTime);

	UFUNCTION(NetMulticast, Reliable)
		void IncreaseKillCount();
		void IncreaseKillCount_Implementation();

	
		//void LoopCount();

public:
	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;
	FTimerHandle roundTimerHandle;
	FTimerHandle startTimerHandle;
	bool isbreakTime = true;

	int serverTime = 15;
	int serverRound = 0;
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
