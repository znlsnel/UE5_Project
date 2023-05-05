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
	UFUNCTION(Server, Reliable)
	void InitializeEnemy(FVector spawnPoint);
	void InitializeEnemy_Implementation(FVector spawnPoint);
	UFUNCTION(NetMulticast, Reliable)
		void InitializeEnemyMulticast(FVector spawnPoint);
	void InitializeEnemyMulticast_Implementation(FVector spawnPoint);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& arr)const override;


public:
	// ��� ����
	void IdleState();
	// �̵� ����

#pragma region MoveState

	UFUNCTION(Client, Reliable)
	void MoveState();
	void MoveState_Implementation();

	//UFUNCTION(NetMulticast, Reliable)
	//void MoveStateMulticast();
	//void MoveStateMulticast_Implementation();
#pragma endregion


	// ���� ����
	void AttackState();
	// �ǰ� ����
	void DamageState();
	// ���� ����
	void DieState();
	// �ǰ� �˸� �̺�Ʈ �Լ�

#pragma region OnDamageProcess

	UFUNCTION(Server, Reliable)
	void OnDamageProcess(int damage);
	void OnDamageProcess_Implementation(int damage);

	UFUNCTION(NetMulticast, Reliable)
	void OnDamageProcessMulticast(int damage);
	void OnDamageProcessMulticast_Implementation(int damage);

#pragma endregion


	void RoundInitEnemy(float bonusAtt, float bonusHp);

	// ���� ��ġ ��������
	void GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastPos(FVector Pos);
	void BroadcastPos_Implementation(FVector Pos);

	void LoopSecond();
	UFUNCTION(Server, Reliable)
	void LoopFindPlayer(const TArray<class ATPSPlayer*> &playerArr, FVector enemyPos);
	void LoopFindPlayer_Implementation(const TArray<class ATPSPlayer*> &playerArr, FVector enemyPos);

	UFUNCTION(NetMulticast, Reliable)
	void FindNearestPlayer(class ATPSPlayer* player);
	void FindNearestPlayer_Implementation(class ATPSPlayer* player);


	// �� ã�� ����� ���� ��ġ
	UPROPERTY(Replicated)
	FVector randomPos;
	

public:
	bool isActive = false;
	FTimerHandle findPlayerTimer;

	// EnemyState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;  

	// ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 2;
	// ��� �ð�
	float currentTime = 0;

	// Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class ATPSPlayer* target;

	// ���� ����
	UPROPERTY()
		class AEnemy* me;

	// ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.f;
	
	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 10;

	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 initHp = 10;

	UPROPERTY(EditAnywhere, Category = FSM)
		float damageDelayTime = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = FSM)
		float dieSpeed = 50.0f;

	// ������� �ִϸ��̼� �������Ʈ
	UPROPERTY()
		class UEnemyAnim* anim;

	// Enumey�� �����ϰ� �ִ�  AIController
	UPROPERTY()
		class AAIController* ai;
	
	TArray<class ATPSPlayer*> players;
	FVector deadLocation = FVector(0, 0, 0);
};
