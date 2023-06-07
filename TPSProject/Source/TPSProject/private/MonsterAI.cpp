// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI.h"
#include "NavigationSystem.h"
#include "MonsterAnim.h"
#include "Monster.h"

#include "Doomstone.h"
#include <Kismet/GameplayStatics.h>


AMonsterAI::AMonsterAI()
{
	//stoneStatue = UGameplayStatics::GetActorOfClass(GetWorld(), ADoomstone::StaticClass());
	//target = stoneStatue;
	//me = Cast<AMonster>(GetOwner());
	//myAnim = Cast<UMonsterAnim>(me->GetMesh()->GetAnimInstance());
 }

void AMonsterAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	StateTickServer();
}

void AMonsterAI::StateTickServer_Implementation()
{
	StateTick();
}

void AMonsterAI::StateTick_Implementation()
{
	switch (MonsterState)
	{
		myAnim->monsterState = MonsterState;
		case EMonsterState::Idle:
			IdleEvent();
			break;
		case EMonsterState::Move:
			MoveEvent();
			break;
		case EMonsterState::Attack:
			AttackEvent();
			break;
		case EMonsterState::Damage:
			DamageEvent();
			break;
		case EMonsterState::Die:
			DieEvent();
			break;

		case EMonsterState::Bictory:
			BictoryEvent();
			break;
	}
}

void AMonsterAI::IdleEvent()
{
	MonsterState = EMonsterState::Move;
}

void AMonsterAI::MoveEvent()
{
	float distance = (target->GetActorLocation() - me->GetActorLocation()).Length();
	if (distance < 50)
	{
		MonsterState = EMonsterState::Attack;
	}
	else
	{
		MoveToActor(target);

	}
}

void AMonsterAI::AttackEvent()
{
	float distance = (target->GetActorLocation() - me->GetActorLocation()).Length();
	if (distance > 50)
	{
		MonsterState = EMonsterState::Move;
	}


}

void AMonsterAI::DamageEvent()
{
}

void AMonsterAI::DieEvent()
{
}

void AMonsterAI::BictoryEvent()
{
}
