// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UENUM(BlueprintType)
enum class EnemyType : uint8
{
	Riktor			UMETA(DisplayName = "Riktor"),
	RiktorFast		UMETA(DisplayName = "RiktorFast"),
	RiktorBig			UMETA(DisplayName = "RiktorBig"),
	RiktorGiant		UMETA(DisplayName = "RiktorGiant"),
	Helix			UMETA(DisplayName = "Helix"),
	Kraken			UMETA(DisplayName = "Kraken"),
	BomberMan		UMETA(DisplayName = "BomberMan"),
	Super			UMETA(DisplayName = "Super"),
};

UENUM(BlueprintType)
enum class EnemyPoolType : uint8
{
	Common			UMETA(DisplayName = "Common"),
	Special		UMETA(DisplayName = "Special"),
	Boss			UMETA(DisplayName = "Boss"),
};



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
	// Called every frame

	virtual void Tick(float DeltaTime) override;

	class ATPSPlayer* player;
	
		void SpawnEnemy(bool SpawnBoss = false);
		int GetSpawnIndex();
		void CreateEnemy(FVector location, EnemyPoolType poolType);
		void AddEnemy(FVector location, EnemyPoolType poolType);
		bool RecycleEnemy(class AEnemy* enemy, int SpawnIndex);

		void FindSpawnPoints();


		TArray<int> preSpawnIndex;
	UFUNCTION(BlueprintCallable)
		void RoundEvent(bool start);
	
		//void LoopCount();

public:
	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;
	FTimerHandle SpawnWaitTimer;
	bool isbreakTime = true;
	int currRound = 0;

	// 랜덤 시간 간격 최솟값
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float minTime = 5.f;

	// 랜덤 시간 간격 최댓값
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float maxTime = 10.5f;

	// 스폰할 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor*> spawnPoints;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> RiktorFactory;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> FastRiktorFactory;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> BigRiktorFactory;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> GiantRiktorFactory;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> KrakenFactory;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> SuperFactory;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy>HelixFactory;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> BomberFactory;

	TArray<class AEnemy*> CommonEnemyPool;
	TArray<class AEnemy*> SpecialEnemyPool;
	TArray<class AEnemy*> BossEnemyPool;


	AEnemy* tempEnemy;
	int32 commonSpawnLimit = 20;
	int32 SpecialSpawnLimit = 8;
	int32 BossSpawnLimit = 2;

	int enemyBonusAttackPower = 0;
	int enemyBonusHp = 0;
};
