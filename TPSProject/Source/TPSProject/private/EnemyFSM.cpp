// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "TPSProject.h"
#include "EnemyAnim.h"
#include "BuildableItem.h"
#include "Doomstone.h"
#include "EnemyHpBar.h"

#include <AIController.h>
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
	isActive = true;
	// 월드에서 ATPSPlayer 타깃 찾아오기\

	me = Cast<AEnemy>(GetOwner());

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	anim->me = me;
	ai = Cast<AAIController>(me->GetController());
	stoneStatue = UGameplayStatics::GetActorOfClass(GetWorld(), ADoomstone::StaticClass());
	target = stoneStatue;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

		case EEnemyState ::Damage:
			DamageState();			
			break;

		case EEnemyState::Die:
			DieState(); 
			break;
		case EEnemyState::Bictory:
			Bictory();
			break;
	}
}

void UEnemyFSM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UEnemyFSM::InitializeEnemy(FVector spawnPoint)
{
	spawnPoint.Z += 100;
	anim->isDead = false;
	isActive = true;
	hp = maxHp;
	me->SetActorHiddenInGame(false);

	me->SetActorLocation(spawnPoint);
	me->SetActorRotation(FRotator(0, 0, 0));
	mState = EEnemyState::Idle;
	anim->animState = mState;
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	anim->InitializeEnemy();
	ai = Cast<AAIController>(me->GetController());
	UpdageTargetTick();
	
	anim->speed = me->GetCharacterMovement()->MaxWalkSpeed;
	if (anim->Montage_IsPlaying(anim->AM_Damaged))
		anim->Montage_Stop(0, anim->AM_Damaged);

}




void UEnemyFSM::IdleState()
{

	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}

	if (currentTime == 0.f)
		currentTime = GetWorld()->GetTimeSeconds();

	if (GetWorld()->GetTimeSeconds() - currentTime > idleDelayTime)
	{
		mState = EEnemyState::Move;
		anim->animState = mState;
		currentTime = 0.f;
	}
}

void UEnemyFSM::Bictory()
{
	if (ai)
		ai->StopMovement();
}






void UEnemyFSM::MoveState()
{
	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}

	if (target == nullptr) {
		target = stoneStatue;
	}

	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	AAIController* cont = Cast<AAIController>(me->GetController());

	MoveEnemy(destination, dir);
}




void UEnemyFSM::MoveEnemy(FVector destination, FVector dir)
{
	//if (GetNetMode() == NM_DedicatedServer) return;

	//me->AddMovementInput(dir.GetSafeNormal());
	//ai->MoveToLocation(destination);

	// NavigationSystem 객체 얻어오기
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	 
	if (ns == nullptr) return;

	// 목적지 길 찾기 경로 데이터 검색
	// FindPathSync()함수는 FPathFindingQuery 구조체 정보가 필요합니다. 
	// 이 쿼리(질의문)을 내비게이션 시스템에 전달해 길 찾기 정보를 찾아내려고 합니다. 이를 위해 FPathFindingQuery 타입의 변수를 query를 선언

	FPathFindingQuery query;
	FAIMoveRequest req;

	// 목적지에서 인지할 수 있는 거리
	req.SetAcceptanceRadius(100);

	req.SetGoalLocation(destination);

	// 길 찾기를 위한 쿼리 생성
	ai->BuildPathfindingQuery(req, query);

	// 길 찾기 결과 가져오기
	FPathFindingResult r = ns->FindPathSync(query);

	
	// 목적지 까지의 길찾기 성공 여부 확인
	if (r.Result == ENavigationQueryResult::Success)
	{
		// 타깃쪽으로 이동
		ai->MoveToLocation(destination);
		//ai->MoveToActor(target);
		//ai->MoveTo(req, &r.Path);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("failed"));

		ai->StopMovement();
		mState = EEnemyState::Idle;
		anim->animState = mState;
	}


	float tempRange = attackRange;
	// 타깃과 가까워지면 공격 상태로 전환
	if (target->ActorHasTag("DoomStone"))
		tempRange += 250;

	if (dir.Length() < tempRange)
	{
		ai->StopMovement();

		mState = EEnemyState::Attack;
		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}
	else {
		mState = EEnemyState::Attack;
		anim->animState = mState;
	}

	if (target == nullptr) return;

	
	// 목표 : 일정 시간에 한 번씩 공격하고 싶다.
	// 1. 시간이 흘러야 한다.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 공격 시간이 됐으니깝
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		anim->bAttackPlay = true;

	}
	anim->bAttackPlay = true;

	// 타깃과의 거리 체크
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	float tempRange = attackRange;
	// 타깃과 가까워지면 공격 상태로 전환
	if (target->ActorHasTag("DoomStone"))
		tempRange += 250;

	anim->target = target;

	// 타깃과의 거리가 공격 범위를 벗어낫는지
	if (target->ActorHasTag("Barricade") == false && distance > tempRange)
	{
		mState = EEnemyState::Move;
		anim->animState = mState;
	}
}

void UEnemyFSM::DamageState()
{
	ai->StopMovement();
	mState = EEnemyState::Idle;
	anim->animState = mState;
	currentTime = 0;
}



void UEnemyFSM::DeadEneny(class ATPSPlayer* player)
{
	anim->isDead = true;
	mState = EEnemyState::Die;
	anim->animState = mState;

	anim->PlayDamageAnim(TEXT("Die"));
	currentTime = 0;
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	int money = 10;
	if (player) {
		player->GetMineralGrace(mineral, grace);
		me->DieEvent(player);
	}

	//me->IncreaseKillCount();
	deadLocation = me->GetActorLocation();
	GetWorld()->GetTimerManager().ClearTimer(UpdateTargetTimer);
}

void UEnemyFSM::DieState()
{
	if (anim->isDead)
	{
		FVector P0 = me->GetActorLocation();
		FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		me->SetActorLocation(P);
		if (deadTime == 0.f)
			deadTime = GetWorld()->GetTimeSeconds();


	}


	if (GetWorld()->GetTimeSeconds() - deadTime > 3.f)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("time %f"), GetWorld()->GetTimeSeconds() - deadTime));
		me->SetActorHiddenInGame(true);
		deadTime = 0.f;

		mState = EEnemyState::Idle;
		anim->animState = mState;
;		if (isActive == false) return;
		isActive = false;
	}

}


void UEnemyFSM::OnDamageProcess(int damage, ATPSPlayer* player,  FName boneName)
{
	if (boneName == FName("head"))
		damage *= 2;

	else if (boneName == FName("spine_03") || boneName == "")
		damage *= 1;

	else 
		damage = damage - (damage / 4);

	damage = UKismetMathLibrary::RandomIntegerInRange(FMath::Max(1, damage - (damage / 3)), damage + (damage / 3));

	if (hp <= 0 || mState == EEnemyState::Die) {
		mState = EEnemyState::Die;
		anim->animState = EEnemyState::Die;
		return;
	}


	float preHp = hp;
	hp -= damage;
	hp = FMath::Max(hp, 0);

	me->AddWorldDamageUI(damage, (float)hp/(float)maxHp, preHp/(float)maxHp);
	

	//me->HpBar->UpdateHpBar(this);

	anim->playHitSound(hp == 0);
	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		currentTime = 0;

		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::FString::Printf(TEXT("Damege%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
		
	}
	else
	{
		DeadEneny(player);
	}


}


void UEnemyFSM::RoundInitEnemy(int bonusAtt, int bonusHp, int round)
{
	if (round == currRound)
		return;

	InitHp += bonusHp;
	anim->initAttackDamage = attackPower + bonusAtt;

	currRound = round;
}



void UEnemyFSM::SetTarget(AActor* targetActor)
{
	if (targetActor->ActorHasTag("Player")){
		if ((me->GetActorLocation() - targetActor->GetActorLocation()).Length() > 500) return;
	}
	else if (targetActor->ActorHasTag("Barricade")) {
		if (Cast<ABuildableItem>(targetActor)->isDestroy) {
			mState = EEnemyState::Attack;
			anim->animState = EEnemyState::Attack;
			return;
		}


	}

	target = targetActor;
}

void UEnemyFSM::UpdageTargetTick()
{
	if (target == nullptr) return;


	if (target->ActorHasTag("Player")) {
		if (Cast<ATPSPlayer>(target)->isDie == false &&
			(target->GetActorLocation() - me->GetActorLocation()).Length() < 500) {
			return;
		}
	}

	if (target->ActorHasTag("BuildableItem")) {
		if (Cast<ABuildableItem>(target)->isDestroy == false && target->IsHidden() == false)
			return;
	}

	target = stoneStatue;

	//GetWorld()->GetTimerManager().ClearTimer(UpdateTargetTimer);
	GetWorld()->GetTimerManager().SetTimer(UpdateTargetTimer, this, &UEnemyFSM::UpdageTargetTick, 1.f, true);
}





