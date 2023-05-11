// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"

#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemyState animState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay = false;

	UPROPERTY(BlueprintReadOnly)
		class AEnemy* me;
	UPROPERTY(BlueprintReadOnly)
		bool isWin = false;
	// ���� �ִϸ��̼� ������ �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category = FSMEvent)
		void OnEndAttackAnimation();

	// �ǰ� �ִϸ��̼� ��� �Լ�
	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
		void PlayDamageAnim(FName sectionName);

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
		void InitializeEnemy();

	UPROPERTY(EditAnywhere, blueprintReadWrite, category = FSM)
		bool isDead = false;

	UPROPERTY(BlueprintReadOnly)
		int AttackDamage = 5;
	int initAttackDamage = 5;
	class ATPSPlayer* target;
};
