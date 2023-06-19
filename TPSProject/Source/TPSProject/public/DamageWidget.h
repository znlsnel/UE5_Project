// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UDamageWidget(const FObjectInitializer& ObjectInitialize);


	UFUNCTION(BlueprintImplementableEvent)
		void OpenWidget();

	void InitDamageWidget(FVector EnemyWorldLocation, int damage);
	void SetLocationLoop();

public:
	FTimerHandle loopSetPositionTimer;
	FTimerHandle initTimer;
	class APlayerController* playerController;
	FVector EnemyPos = FVector(0);

	UPROPERTY(BlueprintReadOnly)
		int Damage = 0;

	UPROPERTY(BlueprintReadWrite)
		bool isInView = false;
};
