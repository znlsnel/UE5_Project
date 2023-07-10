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

	// Save Function
public:
	void SaveGame(FString SaveSlotName, int slotNumber);
	bool LoadGame(int slotNumber);
	void SaveFileDuplicate(class UMySaveGame* saveGame, bool Load = false);
	void UpdateJsonFile(FString SaveSlotName, int slotNumber, int currDay);
	FString ConvertSlotNumberToString(int slotNumber);
	FString GetSlotName(int SlotNumber);
	void GetFieldItem(TArray<struct FFieldItem>& fieldItems);
	void SetFieldItem(TArray<struct FFieldItem>& fieldItems);

public:
	FTimerHandle LoadTimer;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyManager> enemyManagerFactory;
	UPROPERTY(EditAnywhere, blueprintReadOnly, Category = SpawnSettings)
		TSubclassOf<class AMonster> monsterFactory;
		AActor* tempActor;

	//UPROPERTY(Replicated)
	TArray<AActor*> managedActors;
	TArray<class ABuildableItem*> loadActors;


	UPROPERTY(Replicated, BlueprintReadOnly)
		class AEnemyManager* EnemyManager;
	FTimerHandle StartTimer;
	class ATPSPlayer* myPlayer;

	FString firstSaveSlotName = "";
	FString SecondSaveSlotName = "";
	FString ThirdSaveSlotName = "";

};
