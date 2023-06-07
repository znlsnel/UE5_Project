// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UBTTask_FindTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector K_Target;	

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector K_StoneStatue;
};
