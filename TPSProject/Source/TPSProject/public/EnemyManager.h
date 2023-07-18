// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

//UENUM(BlueprintType)
//enum class EnemyType : uint8
//{
//	Riktor			UMETA(DisplayName = "Riktor"),
//	RiktorFast		UMETA(DisplayName = "RiktorFast"),
//	RiktorBig			UMETA(DisplayName = "RiktorBig"),
//	RiktorGiant		UMETA(DisplayName = "RiktorGiant"),
//	Helix			UMETA(DisplayName = "Helix"),
//	Kraken			UMETA(DisplayName = "Kraken"),
//	BomberMan		UMETA(DisplayName = "BomberMan"),
//	Super			UMETA(DisplayName = "Super"),
//};

//UENUM(BlueprintType)
//enum class EnemyPoolType : uint8
//{
//	Common			UMETA(DisplayName = "Common"),
//	Special		UMETA(DisplayName = "Special"),
//	Boss			UMETA(DisplayName = "Boss"),
//};



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

	void SpawnEnemy();
	void SpawnBossMonster();
	int GetSpawnIndex();
	void CreateEnemy(FVector location , TSubclassOf<class AEnemy>& enemyFactory, TArray<class AEnemy*>& pool);
	void AddEnemy(FVector location, TArray<class AEnemy*>& pool);
	bool RecycleEnemy(class AEnemy* enemy, int SpawnIndex);

	void FindSpawnPoints();
	void ResetEnemy();

		TArray<int> preSpawnIndex;
	UFUNCTION(BlueprintCallable)
		void RoundEvent(bool start);
	
		//void LoopCount();

public:
	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;
	FTimerHandle bossSpawnTimerHandle;
	FTimerHandle SpawnWaitTimer;
	FTimerHandle resetTimer;

	bool jumpRoundEvent = false;
	bool isbreakTime = true;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float InitMonsterSpawnTime = 7.f;
	float currMonsterSpawnTimer = 10.f;
	float BossSpawnTime = 60.f;

	// 스폰할 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor*> spawnPoints;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> RiktorFactory;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> KwangFactory;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> Parasite;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> Yaku;

	TArray<class AEnemy*> RiktorPool;
	TArray<class AEnemy*> KwangPool;
	TArray<class AEnemy*> EnemyPool;


	//TArray<class AEnemy*> SpecialEnemyPool;
	//TArray<class AEnemy*> BossEnemyPool;


	AEnemy* tempEnemy;
	UPROPERTY(EditDefaultsOnly)
		int32 spawnLimit = 40;
	UPROPERTY(EditDefaultsOnly)
		int32 bossSpawnLimit = 10;
	bool spawnRiktor = true;

	class AmyGameStateBase* myGameState;
	class ATPSProjectGameModeBase* myGameMode;
};
