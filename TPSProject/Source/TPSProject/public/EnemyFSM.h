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
	Damage,
	Die,
	Bictory,
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
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	void InitializeEnemy(FVector spawnPoint);
	void SetEnemySize();
public:

	void IdleState();
	void Bictory();


#pragma region MoveState

	void MoveState();
	void MoveEnemy(FVector desination, FVector dir);

#pragma endregion

	void AttackState();
	void DamageState();
	void DeadEneny(class ATPSPlayer* player);
	void DieState();


	// 피격 알림 이벤트 함수

#pragma region OnDamageProcess

	void OnDamageProcess(int damage, ATPSPlayer* player);

#pragma endregion
	void RoundInitEnemy(int  bonusAtt, int bonusHp, int round);
	int currRound = 0;
	void SetTarget(AActor* targetActor);
	void UpdageTargetTick();

	// 길 찾기 수행시 랜덤 위치
	UPROPERTY(Replicated)
	FVector randomPos;
	
	bool isAttacked = false;
public:
	bool isActive = false;
	FTimerHandle UpdateTargetTimer;

	// EnemyState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;  

	// 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 0.5f;
	// 경과 시간
	float currentTime = 0;

	// 타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AActor* target;
	class AActor* stoneStatue;

	// 소유 액터
	UPROPERTY()
		class AEnemy* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.f;
		float InitAttackRange = 150.f;
	
	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 10;
	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 maxHp = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 InitHp = 10;

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 MaxMineral = 20;
		int32 mineral = 20;
	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 MaxGrace = 50;
		int32 grace = 50;


	UPROPERTY(EditAnywhere, Category = FSM)
		float damageDelayTime = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = FSM)
		float dieSpeed = 10.0f;

	// 사용중인 애니메이션 블루프린트
	UPROPERTY()
		class UEnemyAnim* anim;

	// Enumey를 소유하고 있는  AIController
	UPROPERTY()
		class AAIController* ai;
	
	FVector deadLocation = FVector(0, 0, 0);
};
