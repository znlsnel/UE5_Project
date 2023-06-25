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
	UEnemyHpBar(const FObjectInitializer& ObjectInitialize);

	UFUNCTION(BlueprintImplementableEvent)
		void OpenWidget();

	UFUNCTION(BlueprintImplementableEvent)
		void CloseWidget();

	void InitHpWidget(class AEnemy* enemy, float currHpPct, float preHpPct);
	void SetLocationLoop();
public:
	FTimerHandle LoopTimer;
	FTimerHandle InitTimer;
	class APlayerController* playerController;

	UPROPERTY(BlueprintReadWrite)
		bool isInView = false;

	class AEnemy* myEnemy;

	UPROPERTY(BlueprintReadOnly)
		float currHpPercent;

	UPROPERTY(BlueprintReadOnly)
		float preHpPercent;

	float uiRenderTime = 2.f;
	
};


