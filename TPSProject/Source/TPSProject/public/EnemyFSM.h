// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Die,
	Stun,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeEnemy(FVector spawnPoint);

	void IdleState();

	void MoveState();
	bool isPossibleToMove(AActor* goalTarget = nullptr);

	void AttackState();
	void DeadEneny(AActor* attacker);
	void DieState();
	void Reset();

	void OnDamageProcess(int damage, AActor* attacker, FName boneName, bool ignoreHpEvent = false);
	void RoundInitEnemy(int round);
	void UpdageTargetTick();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;
	FTimerHandle UpdateTargetTimer;

	class AActor* target;
	class ADoomstone* stoneStatue;
	class UEnemyAnim* anim;
	class AAIController* ai;
	class AEnemy* me;

	UPROPERTY(EditDefaultsOnly, Category = Idle)
		float idleDelayTime = 0.2f;
	float IdleStartTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category = Attack)
		float attackDelayTime = 1.f;
	float lastAttackTime = 0.f;

	UPROPERTY(EditAnywhere, Category = Attack)
		int attackPower = 5;

	UPROPERTY(EditAnywhere)
		float dieSpeed = 10.0f;
	float lastDeadTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = Hp)
		int InitHp = 10;
	int hp = 10;
	int maxHp = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Reward)
		int rwdMineral = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Reward)
		int rwdGrace = 50;

	int currRound = 0;


	bool isActive = false;
	bool isActiveUpdateTargetTick = false;
	bool isInAttackRange = false; 
};
