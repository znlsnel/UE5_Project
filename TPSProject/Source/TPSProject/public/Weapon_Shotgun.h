// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AWeapon_Shotgun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AWeapon_Shotgun();
	virtual void SynchronizeWhitPlayer(class ATPSPlayer* player)override;
};
