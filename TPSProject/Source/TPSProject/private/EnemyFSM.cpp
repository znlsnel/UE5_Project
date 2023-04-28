// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "TPSProject.h"
#include "EnemyAnim.h"

#include <AIController.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/Pawn.h>

#include <NavigationSystem.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	isActive = true;
	// ���忡�� ATPSPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	target = Cast<ATPSPlayer>(actor);

	me = Cast<AEnemy>(GetOwner());

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	anim->me = me;
	ai = Cast<AAIController>(me->GetController());
	
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

	}
	 

	// ...
}

void UEnemyFSM::InitializeEnemy(FVector spawnPoint)
{
	spawnPoint.Z += 100;
	anim->isDead = false;
	isActive = true;
	hp = initHp;
	me->SetActorHiddenInGame(false);
	me->SetActorLocation(spawnPoint);
	me->SetActorRotation(FRotator(0, 0, 0));
	mState = EEnemyState::Idle;
	anim->animState = mState;
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	anim->InitializeEnemy();
}

void UEnemyFSM::IdleState()
{
	// �ð��� �귶���ϱ� 
	currentTime += GetWorld()->DeltaTimeSeconds;
	// ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		// 3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;

		anim->animState = mState;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
} 

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal());
	//ai->MoveToLocation(destination);

	// NavigationSystem ��ü ������
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// ������ �� ã�� ��� ������ �˻�
	// FindPathSync()�Լ��� FPathFindingQuery ����ü ������ �ʿ��մϴ�. 
	// �� ����(���ǹ�)�� ������̼� �ý��ۿ� ������ �� ã�� ������ ã�Ƴ����� �մϴ�. �̸� ���� FPathFindingQuery Ÿ���� ������ query�� ����
	FPathFindingQuery query;
	FAIMoveRequest req;

	// ���������� ������ �� �ִ� �Ÿ�
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	// �� ã�⸦ ���� ���� ����
	ai->BuildPathfindingQuery(req, query);

	// �� ã�� ��� ��������
	FPathFindingResult r = ns->FindPathSync(query);

	// ������ ������ ��ã�� ���� ���� Ȯ��
	if (r.Result == ENavigationQueryResult::Success)
	{
		// Ÿ�������� �̵�
		ai->MoveToLocation(destination);
	}
	else
	{
		// ������ ��ġ�� �̵�
		auto result = ai->MoveToLocation(randomPos);
		
		// �������� �����ϸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// ���ο� ���� ��ġ ��������
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}


	// Ÿ��� ��������� ���� ���·� ��ȯ
	if (dir.Length() < attackRange)
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
	// ��ǥ : ���� �ð��� �� ���� �����ϰ� �ʹ�.
	// 1. �ð��� �귯�� �Ѵ�.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Attack!!!"), true, true, FLinearColor::Green, 2.0f);
		currentTime = 0;

		anim->bAttackPlay = true;
	}
	
	// Ÿ����� �Ÿ� üũ
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	
	// Ÿ����� �Ÿ��� ���� ������ �������
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		anim->animState = mState;
	}
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
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
	

	if (me->GetActorLocation().Z < -200.0f)
	{
		if (isActive == false) return;
		isActive = false;
		me->SetActorHiddenInGame(true);
	}
	anim->animState = mState;
}

void UEnemyFSM::OnDamageProcess(int damage)
{
	if (mState == EEnemyState::Die) return;

	int randDamage = UKismetMathLibrary::RandomIntegerInRange(FMath::Max(1, damage - (damage / 3)), damage + (damage / 3));

	FRotator tempRot = target->GetActorRotation();
	tempRot.Yaw *= -1;
	tempRot.Pitch = 0;

	me->AddWorldDamageUI(tempRot, randDamage);
	hp -= randDamage;
	hp = FMath::Max(hp, 0);

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
		mState = EEnemyState::Die;
		anim->PlayDamageAnim(TEXT("Die"));
		currentTime = 0;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		int money = 10;
		target->GetMoney(money);
		me->DieEvent();
	}

	
}

void UEnemyFSM::RoundInitEnemy(float bonusAtt, float bonusHp)
{
	hp = initHp * bonusHp;
	anim->AttackDamage = anim->initAttackDamage + bonusAtt;
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;

	return result;
}

