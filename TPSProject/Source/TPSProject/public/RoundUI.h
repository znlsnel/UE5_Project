// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API URoundUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URoundUI(const FObjectInitializer& ObjectInitializer);;
	UPROPERTY(BlueprintReadOnly)
		int Round = 0;
	
	UPROPERTY(BlueprintReadWrite)
		int KillCount = 0;
	
	UPROPERTY(BlueprintReadOnly)
		int roundTime = 30;

	UPROPERTY(BlueprintReadOnly)
		bool isEndBreakTime;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateKillCount();

};
