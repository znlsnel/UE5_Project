// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindTarget.h"
#include "MonsterAI.h"
#include "NavigationSystem.h"
#include "Doomstone.h"

#include <Kismet/GameplayStatics.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTTask_FindTarget::UBTTask_FindTarget()
{
	NodeName = TEXT("FindTarget");
}

EBTNodeResult::Type UBTTask_FindTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Monster = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(Monster) == false) {
		return EBTNodeResult::Failed;
	}
	
	//UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Monster->GetWorld());
	//if (IsValid(NavSystem) == false) {
	//	return EBTNodeResult::Failed;
	//}
	
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(K_Target.SelectedKeyName));

	AActor* StoneStatue = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(K_StoneStatue.SelectedKeyName));

	if (IsValid(StoneStatue) == false){
		StoneStatue = UGameplayStatics::GetActorOfClass(Monster->GetWorld(), ADoomstone::StaticClass());
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(K_StoneStatue.SelectedKeyName, StoneStatue);
	}

	if (Target->ActorHasTag("Player")){
		float TargetDistance = (Monster->GetActorLocation() - Target->GetActorLocation()).Length();
		float StatueDistance = (Monster->GetActorLocation() - StoneStatue->GetActorLocation()).Length();

		if (TargetDistance > StatueDistance || StatueDistance < 500) {
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(K_Target.SelectedKeyName, StoneStatue);
		}
		
		return EBTNodeResult::Succeeded;
	}

	else if (Target->ActorHasTag("DoomStone"))
		return EBTNodeResult::Succeeded;

	else{
		if (IsValid(StoneStatue))
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(K_Target.SelectedKeyName, StoneStatue);
	}

	return EBTNodeResult::Type();
}
