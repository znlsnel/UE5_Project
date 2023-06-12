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


#include <NavigationSystem.h>
#include <Net/UnrealNetwork.h>
// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
	// ...
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
}



void UEnemyFSM::IdleState()
{
	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}
	// 시간이 흘렀으니까 
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > idleDelayTime)
	{

		// 3. 이동 상태로 전환하고 싶다.
		mState = EEnemyState::Move;
		// 경과 시간 초기화
		currentTime = 0;

		anim->animState = mState;
		

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
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

	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}
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
	req.SetAcceptanceRadius(10);

	req.SetGoalActor(target);

	if (ai == nullptr) {
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Cont"));
		return;
	}


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
	if (distance > tempRange)
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
	mState = EEnemyState::Die;
	anim->PlayDamageAnim(TEXT("Die"));
	currentTime = 0;
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	int money = 10;
	player->Mineral += 20;
	player->Grace += 50;

	me->DieEvent(player);
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
	}


	if (me->GetActorLocation().Z < deadLocation.Z - 200.0f)
	{
		if (isActive == false) return;
		isActive = false;
		me->SetActorHiddenInGame(true);

	}
	anim->isDead = true;
	mState = EEnemyState::Die;
	anim->animState = mState;
}


void UEnemyFSM::OnDamageProcess(int damage, ATPSPlayer* player)
{
	damage = UKismetMathLibrary::RandomIntegerInRange(FMath::Max(1, damage - (damage / 3)), damage + (damage / 3));


	if (mState == EEnemyState::Die) return;


	FRotator tempRot = player->GetActorRotation();
	tempRot.Yaw *= -1;
	tempRot.Pitch = 0;

	me->AddWorldDamageUI(tempRot, damage);
	hp -= damage;
	hp = FMath::Max(hp, 0);

	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		currentTime = 0;

		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::FString::Printf(TEXT("Damege%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
		me->HpBar->UpdateHpBar(this);
	}
	else
	{
		DeadEneny(player);
	}


}


void UEnemyFSM::RoundInitEnemy(float bonusAtt, float bonusHp)
{
	hp = maxHp * bonusHp;
	anim->AttackDamage = anim->initAttackDamage + bonusAtt;
}

void UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);

	dest = loc.Location;
	return;
}



void UEnemyFSM::SetTarget(AActor* targetActor)
{
	if (targetActor->ActorHasTag("Player")){
		if ((me->GetActorLocation() - targetActor->GetActorLocation()).Length() > 500) return;
	}
	target = targetActor;
}

void UEnemyFSM::UpdageTargetTick()
{
	if (target != nullptr) return;

	if (target->ActorHasTag("Player")) {
		if (Cast<ATPSPlayer>(target)->isDie == false &&
			(target->GetActorLocation() - me->GetActorLocation()).Length() < 500) {
			return;
		}
	}

	if (target->ActorHasTag("BuildableItem")) {
		if (Cast<ABuildableItem>(target)->isDestroy == false)
			return;
	}

	target = stoneStatue;

	GetWorld()->GetTimerManager().ClearTimer(UpdateTargetTimer);
	GetWorld()->GetTimerManager().SetTimer(UpdateTargetTimer, this, &UEnemyFSM::UpdageTargetTick, 1.f, true);
}





