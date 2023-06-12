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
UEnemyFSM::UEnemyFSM() : Super()
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

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("spawnPoint : %f, %f, %f"),  spawnPoint.X, spawnPoint.Y, spawnPoint.Z));


	me->SetActorLocation(spawnPoint);
	me->SetActorRotation(FRotator(0, 0, 0));
	mState = EEnemyState::Idle;
	anim->animState = mState;
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	anim->InitializeEnemy();
	ai = Cast<AAIController>(me->GetController());
	UpdageTargetTick();

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Init Enemy"));
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

	// NavigationSystem ��ü ������
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	 
	if (ns == nullptr) return;

	// ������ �� ã�� ��� ������ �˻�
	// FindPathSync()�Լ��� FPathFindingQuery ����ü ������ �ʿ��մϴ�. 
	// �� ����(���ǹ�)�� ������̼� �ý��ۿ� ������ �� ã�� ������ ã�Ƴ����� �մϴ�. �̸� ���� FPathFindingQuery Ÿ���� ������ query�� ����

	FPathFindingQuery query;
	FAIMoveRequest req;

	// ���������� ������ �� �ִ� �Ÿ�
	req.SetAcceptanceRadius(10);

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
		//ai->MoveToActor(target);
		//ai->MoveTo(req, &r.Path);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Find"));
		ai->StopMovement();
		mState = EEnemyState::Idle;
		anim->animState = mState;
	}


	float tempRange = attackRange;
	// Ÿ��� ��������� ���� ���·� ��ȯ
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


	// ��ǥ : ���� �ð��� �� ���� �����ϰ� �ʹ�.
	// 1. �ð��� �귯�� �Ѵ�.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		anim->bAttackPlay = true;

	}
	anim->bAttackPlay = true;

	// Ÿ����� �Ÿ� üũ
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	float tempRange = attackRange;
	// Ÿ��� ��������� ���� ���·� ��ȯ
	if (target->ActorHasTag("DoomStone"))
		tempRange += 250;

	anim->target = target;

	// Ÿ����� �Ÿ��� ���� ������ �������
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
		
		// hpbar - null
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



void UEnemyFSM::SetTarget(AActor* targetActor)
{
	if (targetActor->ActorHasTag("Player")){
		if ((me->GetActorLocation() - targetActor->GetActorLocation()).Length() > 500) return;
	}
	target = targetActor;
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Set Target!"));
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





