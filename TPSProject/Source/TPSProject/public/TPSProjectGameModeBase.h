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

	UFUNCTION(Server, Reliable)
		void StartGameInServer();
		void StartGameInServer_Implementation();
		UFUNCTION(NetMulticast, Reliable)
			void StartGame();
			void StartGame_Implementation();

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyManager> enemyManagerFactory;

	UPROPERTY(Replicated)
	class AEnemyManager* EnemyManager;

};
