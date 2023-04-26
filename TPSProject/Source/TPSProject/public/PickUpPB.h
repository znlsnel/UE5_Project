// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickUpPB.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPickUpPB : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadOnly)
		class UPickupManager* pickupManager;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void PlayProgressBar();

	UFUNCTION(BlueprintImplementableEvent)
		void StopProgressBar();

};
