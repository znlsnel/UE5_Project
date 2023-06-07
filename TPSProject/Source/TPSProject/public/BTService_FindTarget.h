// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UBTService_FindTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_FindTarget();

	void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds);

	FVector GetMovementLocation(APawn* me, AActor* Target);

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector K_Target;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector K_StoneStatue;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector K_isAttackRange;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector K_MovementLocation;
};
