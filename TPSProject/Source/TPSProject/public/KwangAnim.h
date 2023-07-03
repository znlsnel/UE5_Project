// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnim.h"
#include "KwangAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UKwangAnim : public UEnemyAnim
{
	GENERATED_BODY()
	
public:
	virtual void PlayAttackAnim(bool isLongRangeAttack = false, bool startMotion = false)override;
	virtual void PlayDamageAnim(bool IsDeath, AActor* attacker = nullptr)override;
	virtual void LongRangeAttack()override;

	UFUNCTION()
		void AnimNotify_OnAttackRangeIndicator();

	UFUNCTION()
		void AnimNotify_OnSkill();

	UFUNCTION()
		void AnimNotify_LightningStrike();

	UFUNCTION()
		void AnimNotify_LightningStrikeEnd();

public:
	class AAttackRangeIndicator* myIndicator;


};
