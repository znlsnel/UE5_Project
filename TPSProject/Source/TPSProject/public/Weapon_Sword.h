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

	void AttackEvent();
	
	void BlockAttack();
	void OnBlocking(bool On);
	void ReflectAttack(class AEnemy* enemy);
public:
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* swordEffect;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* ShieldEffect;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* BlockEffect;

	UPROPERTY(EditAnywhere)
		USoundBase* BlockSound;

	UPROPERTY(EditAnywhere)
		USoundBase* ShieldSound;

	bool SwordMoveOn = false;
	bool isBlocking = false;
	bool isEnemyInSensor = false;

	float lastAttackTime = 0.f;
	float lastBlockingTime = -5.f;
	float nextComboDelay = 2.f;

	float BlockingCoolTime = 3.f;

	int32 currCombo = 0;
	FName Attack_Air = FName("Attack_Air");

	FTimerHandle swordMoveTimer;
	FTimerHandle swordActiveTimer;
	FTimerHandle blockingTimer;

	UPROPERTY(EditAnywhere)
		float swordMoveTime = 0.2f;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AM_Blocking;

	TArray<AActor*> overlappingActors;
};
