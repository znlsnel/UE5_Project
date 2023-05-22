// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "WeaponData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponUI.generated.h"


/**
 * 
 */
UCLASS()
class TPSPROJECT_API UWeaponUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void ChangeWeapon(WeaponType weaponType);

	UFUNCTION(BlueprintImplementableEvent)
		void WidgetOn();

	UFUNCTION(BlueprintImplementableEvent)
		void WidgetOff();

	UFUNCTION(BlueprintCallable)
		WeaponType FindSelectWeaponType();

public:
	UPROPERTY(BlueprintReadWrite)
	class ATPSPlayer* player;

	UPROPERTY(BlueprintReadWrite)
		bool isOpen = false;
};
