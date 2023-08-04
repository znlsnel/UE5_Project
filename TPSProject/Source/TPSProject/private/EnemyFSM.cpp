// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "TPSProject.h"
#include "EnemyAnim.h"
#include "BuildableItem.h"
#include "Doomstone.h"
#include "EnemyHpBar.h"
#include "TPSProjectGameModeBase.h"

#include <AIController.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/Pawn.h>
#include <GameFramework/CharacterMovementComponent.h>


#include <NavigationSystem.h>
#include <Net/UnrealNetwork.h>
// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	//// 월드에서 ATPSPlayer 타깃 찾아오기
	//stoneStatue = Cast< ATPSProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->stoneStatue;

	stoneStatue = Cast<ADoomstone>( UGameplayStatics::GetActorOfClass(GetWorld(), ADoomstone::StaticClass()));
	me = Cast<AEnemy>(GetOwner());
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	ai = Cast<AAIController>(me->GetController());

	anim->me = me;
	target = stoneStatue;
	maxHp = InitHp;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isActive == false) return;



	switch (mState)
	{
		case EEnemyState::Idle:
			IdleState();
			break;

		case EEnemyState::Move:			
			MoveState();
			break;

		case EEnemyState::Attack:
			AttackState(); 
			break;

		case EEnemyState::Die:
			DieState(); 
			break;
	}
	anim->speed = me->GetVelocity().Length();
	anim->animState = mState;

}


void UEnemyFSM::InitializeEnemy(FVector spawnPoint)
{
	spawnPoint.Z += 100;

	ai = Cast<AAIController>(me->GetController());
	isActiveUpdateTargetTick = true;
	isInAttackRange = false;
	mState = EEnemyState::Idle;
	target = stoneStatue;
	isActive = true;
	hp = maxHp;

	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

//	anim->speed = me->GetCharacterMovement()->MaxWalkSpeed;
	anim->speed = me->GetVelocity().Length();
	anim->isDead = false;
	
	me->SetActorHiddenInGame(false);
	me->SetActorLocation(spawnPoint);
	me->SetActorRotation(FRotator(0, 0, 0));

	UpdageTargetTick();
}




void UEnemyFSM::IdleState()
{
	IdleStartTime += GetWorld()->GetDeltaSeconds();
	if (IdleStartTime  > idleDelayTime)
	{
		mState = EEnemyState::Move;
		IdleStartTime = 0.f;
	}
}



void UEnemyFSM::MoveState()
{
	if (target == nullptr)
		return;

	if (anim->Montage_IsPlaying(anim->AM_Skill)) {
		ai->StopMovement();
		return;
	}

	double dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	bool isInRange = dist < 1000;

	if (target->ActorHasTag("Player")) {
		if (anim->bHasAbilitySkill &&  isInRange == false && GetWorld()->GetTimeSeconds() - anim->lastLongRangeSkillUseTime > anim->LongRangeSkillCoolTime) {
			anim->LongRangeAttack();
			anim->lastLongRangeSkillUseTime = GetWorld()->GetTimeSeconds();
		}
	}

	if (stoneStatue && stoneStatue->isDestory) {
		mState = EEnemyState::Idle;
		return;
	}

	if (isInAttackRange) {
		mState = EEnemyState::Attack;
		ai->StopMovement();
	}
	else
		ai->MoveToActor(target);

}



bool UEnemyFSM::isPossibleToMove(AActor* goalTarget)
{
	if (goalTarget == nullptr)
		goalTarget = stoneStatue;

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (ns == nullptr) return false;

	FPathFindingQuery query;
	FAIMoveRequest req;

	req.SetAcceptanceRadius(100);
	req.SetGoalActor(goalTarget);

	ai->BuildPathfindingQuery(req, query);
	FPathFindingResult r = ns->FindPathSync(query);

	if (r.Result == ENavigationQueryResult::Success)
		return true;
	else {
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Failed findPath"));
		return false;
	}
}

void UEnemyFSM::AttackState()
{
	if (isInAttackRange == false) {
		mState = EEnemyState::Idle;
		return;
	}
	if (stoneStatue && stoneStatue->isDestory)
		return;
	if (anim->Montage_IsPlaying(anim->AM_Attack))
		return;

	
	ai->StopMovement();

	lastAttackTime += GetWorld()->GetDeltaSeconds();
	if (lastAttackTime > attackDelayTime){
		lastAttackTime = 0;
		anim->PlayAttackAnim();
	}
}


void UEnemyFSM::DeadEneny(AActor* attacker)
{
	mState = EEnemyState::Die;
	anim->isDead = true;

	anim->PlayDamageAnim(true);
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	int mineral = (int)((1 + ((float)currRound * 0.3f)) * rwdMineral);
	int grace = (int)((1 + ((float)currRound * 0.3f)) * rwdGrace);

	if (attacker && attacker->ActorHasTag("Player")) {
		Cast<ATPSPlayer>(attacker)->GetMineralGrace(mineral, grace);
	}
}

void UEnemyFSM::DieState()
{
	ai->StopMovement();
	if (anim->isDead)
	{
		FVector P0 = me->GetActorLocation();
		FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		me->SetActorLocation(P);
		if (lastDeadTime == 0.f)
			lastDeadTime = GetWorld()->GetTimeSeconds();


	}


	if (GetWorld()->GetTimeSeconds() - lastDeadTime > 3.f)
	{
		Reset();
	}

}

void UEnemyFSM::Reset()
{
	lastDeadTime = 0.f;
	isActive = false;
	mState = EEnemyState::Idle;

	me->AddActorLocalOffset(FVector(0, 0, -1000));
	me->SetActorHiddenInGame(true);
	if (anim->Montage_IsPlaying(anim->AM_Damaged))
		anim->Montage_Stop(0, anim->AM_Damaged);
}


void UEnemyFSM::OnDamageProcess(int damage, AActor* attacker,  FName boneName, bool ignoreHpEvent)
{
	if (anim->isDead)
		return;

	if (IsValid(attacker) && attacker->ActorHasTag(TEXT("Player"))) 
	{
		double dist = FVector::Distance(attacker->GetActorLocation(), me->GetActorLocation());
		double allowDist = (me->targetSensor->GetScaledBoxExtent().Y * me->GetMesh()->GetRelativeScale3D().Y);

		bool isInRange = dist < playerTraceDist;

		if (isInRange) {
			target = attacker;
			//anim->PlayAttackAnim(false, true);
		}

	}

	// Damage
	{
		if (boneName == FName("head"))
			damage *= 2;

		else if (boneName == FName("spine_03") || boneName == "")
			damage *= 1;

		else 
			damage = damage - (damage / 4);

		damage = UKismetMathLibrary::RandomIntegerInRange(
			FMath::Max(1, damage - (damage / 3)),
			damage + (damage / 3)
		);
	}

	float preHp = hp;
	hp -= damage;
	hp = FMath::Max(hp, 0);

	if (ignoreHpEvent == false)
		me->AddWorldDamageUI(damage, (float)hp / (float)maxHp, preHp / (float)maxHp);
	anim->playHitSound(hp == 0);

	me->damageEffect->Activate(true);
	if (hp == 0) {
		mState = EEnemyState::Die;
		isActiveUpdateTargetTick = false;
		DeadEneny(attacker);
		return;
	}
	else {
		if (GetWorld()->GetTimeSeconds() - anim->lastLongRangeSkillUseTime > anim->LongRangeSkillCoolTime) {
			float temp = GetWorld()->GetTimeSeconds() - anim->lastLongRangeSkillUseTime;
			anim->lastLongRangeSkillUseTime -= (temp - anim->LongRangeSkillCoolTime) - 5;
		}
		anim->lastLongRangeSkillUseTime += 2;

		if (anim->bHasAbilitySkill == false) {
			lastAttackTime = 0.f;
		}
		anim->PlayDamageAnim(false , attacker);
	}
}


void UEnemyFSM::RoundInitEnemy(int round)
{
	if (round == currRound)
		return;

	maxHp = InitHp + (int)((float)InitHp * 0.3f * (float)round);
	anim->AttackDamage = attackPower + (int)((float)attackPower * 0.4f * (float)round);

	currRound = round;
}


void UEnemyFSM::UpdageTargetTick()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateTargetTimer);
	if (target == nullptr || isActiveUpdateTargetTick == false)
		return;

	double dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	bool TargetStoneStatue = false;
	if (target->ActorHasTag("Player") ) {
	//	TargetStoneStatue = dist > (me->targetSensor->GetScaledBoxExtent().Y * me->GetMesh()->GetRelativeScale3D().Y) * 10;
		if (Cast<ATPSPlayer>(target)->hp > 0)
			TargetStoneStatue = dist > playerTraceDist;
		else
			TargetStoneStatue = true;
	}
	else if (target->ActorHasTag("BuildableItem")) {
		if (Cast<ABuildableItem>(target)->isDestroy || target->IsHidden())
			TargetStoneStatue = true;
	}

	if (TargetStoneStatue)
		target = stoneStatue;

	GetWorld()->GetTimerManager().SetTimer(UpdateTargetTimer, this, &UEnemyFSM::UpdageTargetTick, 1.f, true);
}





