// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAI.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
	Bictory,
};

UCLASS()
class TPSPROJECT_API AMonsterAI : public AAIController
{
	GENERATED_BODY()

protected:
	AMonsterAI();

public:
	virtual void Tick(float DeltaSeconds)override;
	
	UFUNCTION(Server, Reliable)
		void StateTickServer();
		void StateTickServer_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void StateTick();
		void StateTick_Implementation();


	void IdleEvent();
	void MoveEvent();
	void AttackEvent();
	void DamageEvent();
	void DieEvent();
	void BictoryEvent();

public:
	EMonsterState MonsterState = EMonsterState::Idle;

	class UMonsterAnim* myAnim;
	class AMonster* me;
	AActor* target;
	AActor* stoneStatue;

};


