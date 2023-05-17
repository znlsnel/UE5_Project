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
	// ���忡�� ATPSPlayer Ÿ�� ã�ƿ���\

	TArray<AActor*, FDefaultAllocator> actors; UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATPSPlayer::StaticClass(), actors);

	for (auto player : actors)
		players.Add(Cast<ATPSPlayer>(player));

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
		case EEnemyState::Bictory:
			Bictory();
			break;
	}
	 

	// ...
}

void UEnemyFSM::InitializeEnemy_Implementation(FVector spawnPoint)
{
	InitializeEnemyMulticast(spawnPoint);
}

void UEnemyFSM::InitializeEnemyMulticast_Implementation(FVector spawnPoint)
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
	LoopSecond();
}

void UEnemyFSM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyFSM, randomPos);
}

void UEnemyFSM::IdleState()
{
	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}
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

		BroadcastPos(randomPos);
	}
}

void UEnemyFSM::Bictory()
{
	if (ai)
		ai->StopMovement();
}



//void UEnemyFSM::MoveState_Implementation()
//{
//	MoveStateMulticast();
//
//}
//


void UEnemyFSM::MoveState_Implementation()
{
	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}
	if (target == nullptr) 
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Target!!"));
		return;
	}
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	MoveStateMulticast(destination, dir);
}




void UEnemyFSM::MoveStateMulticast_Implementation(FVector destination, FVector dir)
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

	// NavigationSystem ��ü ������
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (ns == nullptr) return;
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

void UEnemyFSM::AttackState_Implementation()
{
	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}
	else
		mState = EEnemyState::Attack;
	anim->animState = mState;

	if (target)
		AttackMulticast(target);
}

void UEnemyFSM::AttackMulticast_Implementation(ATPSPlayer* player)
{
	
	if (anim->isWin)
	{
		mState = EEnemyState::Bictory;
		anim->animState = mState;
		return;
	}
	else
	mState = EEnemyState::Attack;
	anim->animState = mState;
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
	anim->bAttackPlay = true;

	// Ÿ����� �Ÿ� üũ
	float distance = FVector::Distance(player->GetActorLocation(), me->GetActorLocation());
	

		anim->target = player;

	// Ÿ����� �Ÿ��� ���� ������ �������
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		anim->animState = mState;
	}
}
void UEnemyFSM::DamageState_Implementation()
{
	ai->StopMovement();
	mState = EEnemyState::Idle;
	anim->animState = mState;
	DamageMulti();
}

void UEnemyFSM::DamageMulti_Implementation()
{
	//currentTime += GetWorld()->DeltaTimeSeconds;
	//if (currentTime > damageDelayTime)
	//{
	//	mState = EEnemyState::Idle;
	//	currentTime = 0;
	//	anim->animState = mState;
	//}
	mState = EEnemyState::Idle;
	currentTime = 0;
	anim->animState = mState;
}

void UEnemyFSM::DeadEneny_Implementation(class ATPSPlayer* player)
{
	DeadEnemyMulti(player);
}

void UEnemyFSM::DieState_Implementation()
{
	anim->isDead = true;
	mState = EEnemyState::Die;
	anim->animState = mState;
	DieStateMulticast();
}

void UEnemyFSM::DieStateMulticast_Implementation()
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

void UEnemyFSM::OnDamageProcess_Implementation(int damage, ATPSPlayer* player)
{
	int randDamage = UKismetMathLibrary::RandomIntegerInRange(FMath::Max(1, damage - (damage / 3)), damage + (damage / 3));

	OnDamageProcessMulticast(randDamage, player);
}

void UEnemyFSM::OnDamageProcessMulticast_Implementation(int damage, ATPSPlayer* player)
{
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
	}
	else
	{
		DeadEneny(player);
	}


}

void UEnemyFSM::DeadEnemyMulti_Implementation(class ATPSPlayer* player)
{
	mState = EEnemyState::Die;
	anim->PlayDamageAnim(TEXT("Die"));
	currentTime = 0;
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	int money = 10;
	player->GetMoney(money);
	me->DieEvent(player);
	//me->IncreaseKillCount();
	deadLocation = me->GetActorLocation();
}

void UEnemyFSM::RoundInitEnemy_Implementation(float bonusAtt, float bonusHp)
{
	hp = initHp * bonusHp;
	anim->AttackDamage = anim->initAttackDamage + bonusAtt;
}

void UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	if (GetNetMode() != NM_DedicatedServer)return;
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);

	dest = loc.Location;


	return;
}

void UEnemyFSM::BroadcastPos_Implementation(FVector Pos)
{
	this->randomPos = Pos;
}

void UEnemyFSM::LoopSecond()
{
	if (IsValid(me))
		LoopFindPlayer(players, me->GetActorLocation());

	if (target == nullptr)
	{
		anim->isWin = true;

		mState = EEnemyState::Bictory;
		anim->animState = mState;
			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Bictory"));
		
	}

	GetWorld()->GetTimerManager().SetTimer(findPlayerTimer, this, &UEnemyFSM::LoopSecond, 1.0f);
}

void UEnemyFSM::LoopFindPlayer_Implementation(const TArray<class ATPSPlayer*> &playerArr, FVector enemyPos)
{
	bool isBictory = true;
	ATPSPlayer* closePlayer = nullptr;
	double minLength = 1123123123;

	for (auto player : players)
	{
		if (player->isDie) continue;


		double length = (enemyPos - player->GetActorLocation()).Length();

		if (length < minLength)
		{
			minLength = length;
			closePlayer = player;
		}
		isBictory = false;

	}

	FindNearestPlayer(closePlayer, isBictory);
}


void UEnemyFSM::FindNearestPlayer_Implementation(class ATPSPlayer* player, bool isBictory)
{

 	target = player;

}




//void UEnemyFSM::GetLifeTimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(UEnemyFSM, randomPos);
//
//	DOREPLIFETIME_CONDITION(UEnemyFSM, randomPos, COND_SkipOwner);
//}
//
