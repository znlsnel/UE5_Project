// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatueHpWidget.generated.h"

/**
 * 
 */

UCLASS()
class TPSPROJECT_API UStatueHpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UStatueHpWidget(const FObjectInitializer& ObjectInitialize);
	virtual void NativeConstruct()override;
	void UpdateWidgetPos();
public:
	class APlayerController* playerCtrl;
	class ADoomstone* parent;

	FTimerHandle LoopTimer;

	UPROPERTY(BlueprintReadOnly)
		int Hp = 0;
	UPROPERTY(BlueprintReadOnly)
		int MaxHp = 0;
	UPROPERTY(BlueprintReadOnly)
		float HpPercent = 0.f;
};
