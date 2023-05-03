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

	virtual void BeginPlay()override;
	UFUNCTION(BlueprintCallable)
		void StartGame();
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyManager> enemyManagerFactory;
	class AEnemyManager* EnemyManager;

	UPROPERTY(EditAnywhere, category = testWidget)
		TSubclassOf<UUserWidget> widgetFactory;
	class UUserWidget* testWidget;

	TArray<ULocalPlayer*, FDefaultAllocator> players;


};
