// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindTarget.h"
#include "NavigationSystem.h"
#include "MonsterAI.h"
#include "Doomstone.h"
#include "BuildableItem.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTService_FindTarget::UBTService_FindTarget()
{
	Interval = 1.5f;
	RandomDeviation = 0.3f;
}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ownerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(ownerPawn) == false) 
		return;
	



	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(K_Target.SelectedKeyName));

	ADoomstone* StoneStatue = Cast<ADoomstone>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(K_StoneStatue.SelectedKeyName));

	if (IsValid(StoneStatue) == false) {
		StoneStatue = Cast<ADoomstone>(UGameplayStatics::GetActorOfClass(ownerPawn->GetWorld(), ADoomstone::StaticClass()));
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(K_StoneStatue.SelectedKeyName, StoneStatue);
	}

	if (IsValid(Target) == false) {
		Target = StoneStatue;
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(K_Target.SelectedKeyName, StoneStatue);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(K_isAttackRange.SelectedKeyName, false);
	}
	else if (Target->ActorHasTag("DoomStone")) 
	{
		float StatueDistance = (ownerPawn->GetActorLocation() - StoneStatue->GetActorLocation()).Length();

		if (StatueDistance < 100 && StoneStatue->isDestory == false) 
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(K_isAttackRange.SelectedKeyName, true);

		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(K_isAttackRange.SelectedKeyName, false);
		}
	}

	else if (Target->ActorHasTag("Barricade")) 
	{
		if (Cast<ABuildableItem>(Target)->isDestroy)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(K_isAttackRange.SelectedKeyName, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(K_Target.SelectedKeyName, StoneStatue);
		} 
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(K_isAttackRange.SelectedKeyName, true);
		}

	}

	else if (Target->ActorHasTag("Player")) {
		float TargetDistance = (ownerPawn->GetActorLocation() - Target->GetActorLocation()).Length();
		float StatueDistance = (ownerPawn->GetActorLocation() - StoneStatue->GetActorLocation()).Length();

		if (TargetDistance > StatueDistance || StatueDistance < 500) {
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(K_Target.SelectedKeyName, StoneStatue);
		}
	}


	if (IsValid(Target) && OwnerComp.GetBlackboardComponent()->GetValueAsBool(K_isAttackRange.SelectedKeyName) == false)
	{


		//FVector tempLoc = GetMovementLocation(ownerPawn, Target);
		FVector tempLoc = Target->GetActorLocation();
		tempLoc.Z = 0;

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(K_MovementLocation.SelectedKeyName, tempLoc);

		tempLoc = FVector(0, 0, 0);
		tempLoc = OwnerComp.GetBlackboardComponent()->GetValueAsVector(K_MovementLocation.SelectedKeyName);

		UKismetSystemLibrary::PrintString(ownerPawn->GetWorld(), FString::Printf(TEXT("MovementLocation : %f, %f, %f"), tempLoc.X, tempLoc.Y, tempLoc.Z));
	}
	else
	{
		UKismetSystemLibrary::PrintString(ownerPawn->GetWorld(), TEXT("AttackRange!"));

	}


}



FVector UBTService_FindTarget::GetMovementLocation(APawn* me, AActor* Target)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(me->GetWorld());
	if (IsValid(ns) == false) 
		return FVector();

	FPathFindingQuery query;
	FAIMoveRequest req;

	// 목적지에서 인지할 수 있는 거리
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(Target->GetActorLocation());

	Cast<AAIController>(me->GetController())->BuildPathfindingQuery(req, query);

	// 길 찾기 결과 가져오기
	FPathFindingResult r = ns->FindPathSync(query);

	if (r.Result == ENavigationQueryResult::Success)
	{
		Cast<AAIController>(me->GetController())->MoveToLocation(Target->GetActorLocation());
		UKismetSystemLibrary::PrintString(me->GetWorld(), TEXT("Success!"));
	}
	else
	{
		UKismetSystemLibrary::PrintString(me->GetWorld(), TEXT("Failed!"));

	}
	return FVector();

}
