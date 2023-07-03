// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Riktor.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ARiktor : public AEnemy
{
	GENERATED_BODY()
	

public:
	ARiktor();
	virtual void BeginPlay()override;
	class ARiktorThunder* GetThunder();

	void SpawnThunderMonster();
	void SpawnThunderLoop();

	void OnAttackRangeIndicator(bool trigger);

public:
	FTimerHandle SpawnThunderTimer;
	FTimerHandle SpawnThunderEndTimer;

	bool isSpawnThunder = false;
	int isLeftHand = false;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARiktorThunder> ThunderFactory;
	TArray<class ARiktorThunder*> Thunders;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAttackRangeIndicator> rangeIndicatorFactory;
	class AAttackRangeIndicator* rangeIndicator;

	class AActor* targetPlayer;

};
