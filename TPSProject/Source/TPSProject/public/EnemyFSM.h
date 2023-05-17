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
	UFUNCTION(Server, Reliable)
	void InitializeEnemy(FVector spawnPoint);
	void InitializeEnemy_Implementation(FVector spawnPoint);
	UFUNCTION(NetMulticast, Reliable)
		void InitializeEnemyMulticast(FVector spawnPoint);
	void InitializeEnemyMulticast_Implementation(FVector spawnPoint);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& arr)const override;


public:

	void IdleState();
	void Bictory();


#pragma region MoveState

	UFUNCTION(Server, Reliable)
	void MoveState();
	void MoveState_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void MoveStateMulticast(FVector desination, FVector dir);
	void MoveStateMulticast_Implementation(FVector desination, FVector dir);
#pragma endregion

	UFUNCTION(Server, Reliable)
		void AttackState();
		void AttackState_Implementation();
		UFUNCTION(NetMulticast, Reliable)
			void AttackMulticast(class ATPSPlayer* player);
			void AttackMulticast_Implementation(class ATPSPlayer* player);

	// �ǰ� ����
	UFUNCTION(Server, Reliable)
	void DamageState();
	void DamageState_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void DamageMulti();
		void DamageMulti_Implementation();

		// ���� ����
	UFUNCTION(Server, Reliable)
		void DeadEneny(class ATPSPlayer* player);
		void DeadEneny_Implementation(class ATPSPlayer* player);

		UFUNCTION(NetMulticast, Reliable)
			void DeadEnemyMulti(class ATPSPlayer* player);
			void DeadEnemyMulti_Implementation(class ATPSPlayer* player);


	UFUNCTION(Server, Reliable)
	void DieState();
	void DieState_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void DieStateMulticast();
		void DieStateMulticast_Implementation();
	// �ǰ� �˸� �̺�Ʈ �Լ�

#pragma region OnDamageProcess

	UFUNCTION(Server, Reliable)
	void OnDamageProcess(int damage, ATPSPlayer* player);
	void OnDamageProcess_Implementation(int damage, ATPSPlayer* player);

	UFUNCTION(NetMulticast, Reliable)
	void OnDamageProcessMulticast(int damage, ATPSPlayer* player);
	void OnDamageProcessMulticast_Implementation(int damage, ATPSPlayer* player);

#pragma endregion

	UFUNCTION(Client, Reliable)
	void RoundInitEnemy(float bonusAtt, float bonusHp);
	void RoundInitEnemy_Implementation(float bonusAtt, float bonusHp);

	// ���� ��ġ ��������
	void GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastPos(FVector Pos);
	void BroadcastPos_Implementation(FVector Pos);

	void LoopSecond();
	UFUNCTION(Client, Reliable)
	void LoopFindPlayer(const TArray<class ATPSPlayer*> &playerArr, FVector enemyPos);
	void LoopFindPlayer_Implementation(const TArray<class ATPSPlayer*> &playerArr, FVector enemyPos);

	UFUNCTION(NetMulticast, Reliable)
	void FindNearestPlayer(class ATPSPlayer* player, bool isBictory);
	void FindNearestPlayer_Implementation(class ATPSPlayer* player, bool isBictory);


	// �� ã�� ����� ���� ��ġ
	UPROPERTY(Replicated)
	FVector randomPos;
	
	bool isAttacked = false;
public:
	bool isActive = false;
	FTimerHandle findPlayerTimer;

	// EnemyState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;  

	// ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 0.5f;
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
		float damageDelayTime = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = FSM)
		float dieSpeed = 10.0f;

	// ������� �ִϸ��̼� �������Ʈ
	UPROPERTY()
		class UEnemyAnim* anim;

	// Enumey�� �����ϰ� �ִ�  AIController
	UPROPERTY()
		class AAIController* ai;
	
	TArray<class ATPSPlayer*> players;
	FVector deadLocation = FVector(0, 0, 0);
};
