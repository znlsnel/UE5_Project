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
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
	virtual void SynchronizeWhitPlayer(class ATPSPlayer* player)override;
	virtual void Attack()override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* enemySensor;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* swordEffect;
	
	bool isSwingingSword = false;
	bool SwordMoveOn = false;
	float lastAttackTime = 0.f;
	float nextComboDelay = 2.5f;
	int32 currCombo = 0;
	FName Attack_1 = FName("Attack_1");
	FName Attack_2 = FName("Attack_2");
	FName Attack_3 = FName("Attack_3");
	FName Attack_4 = FName("Attack_4");
	FName Attack_Air = FName("Attack_Air");

	FTimerHandle swordMoveTimer;
	UPROPERTY(EditAnywhere)
		float swordMoveTime = 0.2f;
};
