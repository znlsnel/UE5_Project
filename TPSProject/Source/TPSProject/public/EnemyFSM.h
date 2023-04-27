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
public:
	// 대기 상태
	void IdleState();
	// 이동 상태
	void MoveState();
	// 공격 상태
	void AttackState();
	// 피격 상태
	void DamageState();
	// 죽음 상태
	void DieState();
	// 피격 알림 이벤트 함수
	void OnDamageProcess(int damage);

	// 랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	// 길 찾기 수행시 랜덤 위치
	FVector randomPos;
	

public:
	bool isActive = false;
	
	// EnemyState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;  

	// 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 2;
	// 경과 시간
	float currentTime = 0;

	// 타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class ATPSPlayer* target;

	// 소유 액터
	UPROPERTY()
		class AEnemy* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.f;
	
	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 10;

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 maxHp = 10;

	UPROPERTY(EditAnywhere, Category = FSM)
		float damageDelayTime = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = FSM)
		float dieSpeed = 50.0f;

	// 사용중인 애니메이션 블루프린트
	UPROPERTY()
		class UEnemyAnim* anim;

	// Enumey를 소유하고 있는  AIController
	UPROPERTY()
		class AAIController* ai;

	
};
