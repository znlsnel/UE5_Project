// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "Animation/AnimInstance.h"
#include "Weapon_BowAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UWeapon_BowAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
		BowState bowState = BowState::TravelMode;
};
