// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Kwang.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AKwang : public AEnemy
{
	GENERATED_BODY()
protected:
	AKwang();
	virtual void BeginPlay()override;

public:
	class AAttackRangeIndicator* GetAttackRangeIndicator();
	void LoopLightningStrike();

public:
	FTimerHandle lightningLoopTimer;


	bool isLightningLoop = false;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAttackRangeIndicator> attackRangeIndicatorFactory;
	TArray<class AAttackRangeIndicator*> attackRangeIndicators;

	class UKwangAnim* myAnim;
	class AActor* player;

};
