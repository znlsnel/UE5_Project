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
	virtual void NativeBeginPlay()override;
	// 피격 애니메이션 재생 함수
	virtual void PlayDamageAnim(bool IsDeath, AActor* attacker = nullptr);
	virtual void PlayAttackAnim(bool isLongRangeAttack = false, bool startMotion = false);
	virtual void LongRangeAttack();

	UFUNCTION()
		void AnimNotify_OnAttack();
	UFUNCTION()
		void AnimNotify_DamagedEnd();

	void playHitSound(bool IsDeath);
	void AttackToTargets(TArray<AActor*> actors, int damage);


public:
	UPROPERTY(EditAnywhere, blueprintReadWrite, category = FSM)
		bool isDead = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay = false;

	UPROPERTY(EditAnywhere)
		bool bHasAbilitySkill = false;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AM_Damaged;
	UPROPERTY(EditAnywhere)
		UAnimMontage* AM_Attack;
	UPROPERTY(EditAnywhere)
		UAnimMontage* AM_Skill;
	UPROPERTY(EditAnywhere)
		UAnimMontage* AM_Die;
	
	UPROPERTY(EditAnywhere)
		int attackAMSectionCount = 1;
	int currAttackSection = 1;

	int AttackDamage = 5;
	int SkillDamage = 5;

	UPROPERTY(EditAnywhere)
		float LongRangeSkillCoolTime = 30.f;
	float lastLongRangeSkillUseTime = 0.f;

	UPROPERTY(EditAnywhere)
		float MeleeSkillCoolTime = 15.f;
	float lastMeleeSkillUseTime = 0.f;

	float lastStartMotionTime = -10.f;

	UPROPERTY(BlueprintReadOnly)
		float speed = 100.f;

	UPROPERTY(EditAnywhere)
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere)
		USoundBase* DeathSound;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemyState animState;

	UPROPERTY(BlueprintReadOnly)
		class AEnemy* me;

	UPROPERTY(EditAnywhere)
		FName meleeSkill= "Ability_1";

	UPROPERTY(EditAnywhere)
		FName longRangeSkill = "Ability_2";

};
