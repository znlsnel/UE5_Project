// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon_Gun.h"
#include "Weapon_Pistol.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AWeapon_Pistol : public AWeapon_Gun
{
	GENERATED_BODY()
	
public:
	AWeapon_Pistol();
	virtual void SynchronizeWhitPlayer(class ATPSPlayer* player)override;
	virtual void Attack()override;
};
