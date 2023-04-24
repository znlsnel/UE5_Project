// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoBox.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AAmmoBox : public AItem
{
	GENERATED_BODY()
	

public:
	AAmmoBox();
	bool UseAmmoBox();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AmmoType ammoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ammo;
};
