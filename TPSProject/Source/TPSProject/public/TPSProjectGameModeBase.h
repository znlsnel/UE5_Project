// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ATPSProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	ATPSProjectGameModeBase();
	virtual void InitGame(const FString& mapName, const FString& Options, FString& ErrorMessage)override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;

	virtual void BeginPlay()override;

public:
	void RegisterActor(AActor* actorTpRegister);

	UFUNCTION(Server, Reliable)
		void RegisterActorServer();
		void RegisterActorServer_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void RegisterActorMulti();
		void RegisterActorMulti_Implementation();

	UFUNCTION(Server, Reliable)
		void StartGameInServer();
		void StartGameInServer_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void StartGame();
		void StartGame_Implementation();

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyManager> enemyManagerFactory;
	UPROPERTY(EditAnywhere, blueprintReadOnly, Category = SpawnSettings)
		TSubclassOf<class AMonster> monsterFactory;
		AActor* tempActor;

	//UPROPERTY(Replicated)
	TArray<AActor*> managedActors;

	UPROPERTY(Replicated, BlueprintReadOnly)
		class AEnemyManager* EnemyManager;
	FTimerHandle StartTimer;
};
