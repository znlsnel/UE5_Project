// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_Sword.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AWeapon_Sword : public AWeapon
{
	GENERATED_BODY()
	
public:
	AWeapon_Sword();
	virtual void SynchronizeWhitPlayer(class ATPSPlayer* player)override;
	virtual void Attack()override;


public:
	float lastAttackTime = 0.f;
	float nextComboDelay = 2.5f;
	int32 currCombo = 0;
	FName Attack_1 = FName("Attack_1");
	FName Attack_2 = FName("Attack_2");
	FName Attack_3 = FName("Attack_3");
	FName Attack_4 = FName("Attack_4");
	FName Attack_Air = FName("Attack_Air");
};