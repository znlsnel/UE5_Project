// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAI.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnim.generated.h"

/**
 * 
 */

UCLASS()
class TPSPROJECT_API UMonsterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
		EMonsterState monsterState = EMonsterState::Idle;
};
