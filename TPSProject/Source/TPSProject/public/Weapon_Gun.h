// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_Gun.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AWeapon_Gun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AWeapon_Gun();

	virtual void Attack()override;
	
	void FireWeapon();

	//UFUNCTION(Server, Reliable)
	void createNiagara(FHitResult pHitResult);
};
