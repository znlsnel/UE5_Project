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
		void OnEndAttackAnimation(int Damage = -1);

	// �ǰ� �ִϸ��̼� ��� �Լ�
	void PlayDamageAnim(FName sectionName);

	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_DamagedEnd();
	UFUNCTION()
		void AnimNotify_DieENd();
	void playHitSound(bool IsDeath);


	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
		void InitializeEnemy();

	UPROPERTY(EditAnywhere, blueprintReadWrite, category = FSM)
		bool isDead = false;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AM_Damaged;
	UPROPERTY(EditAnywhere)
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere)
		USoundBase* DeathSound;

	UPROPERTY(BlueprintReadOnly)
		int AttackDamage = 5;
	int initAttackDamage = 5;

	UPROPERTY(BlueprintReadOnly)
	float speed = 100.f;
	class AActor* target;
};
