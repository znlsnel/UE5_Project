// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Initialization(class ATPSPlayer* player);

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeCrosshair(WeaponType weaponType);

	UFUNCTION(BlueprintImplementableEvent)
		void AttackCrosshair(bool isPressed);

	UFUNCTION(BlueprintImplementableEvent)
		void MoveCrosshair();

public:
	UPROPERTY(BlueprintReadWrite)
		class ATPSPlayer* myPlayer;
};
