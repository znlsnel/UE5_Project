// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponData.h"
#include "Weapon_Bow.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AWeapon_Bow : public AWeapon
{
	GENERATED_BODY()
public:
	AWeapon_Bow();
	virtual void SynchronizeWhitPlayer(class ATPSPlayer* player)override;
	virtual void Attack()override;
	void Attack(bool isPress);

public:
	BowState bowState = BowState::TravelMode;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* BowActiveAM;

	FName DrawBow = FName("Draw");
	FName DrawCancel = FName("Cancel");

};
