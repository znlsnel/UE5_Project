// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHpBar.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UEnemyHpBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHpBar(class UEnemyFSM* fsm);

public:
	UPROPERTY(BlueprintReadWrite)
		class UEnemyFSM* enemyFsm;

	
};


