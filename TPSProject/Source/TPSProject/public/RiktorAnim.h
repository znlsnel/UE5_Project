// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnim.h"
#include "RiktorAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API URiktorAnim : public UEnemyAnim
{
	GENERATED_BODY()
	

public:
	virtual void LongRangeAttack()override;

	UFUNCTION()
		void AnimNotify_OnRangeIndicator();

	UFUNCTION()
		void AnimNotify_TriggerRangeIndicator();

	UFUNCTION()
		void AnimNotify_SpawnThunderMonster();
};
