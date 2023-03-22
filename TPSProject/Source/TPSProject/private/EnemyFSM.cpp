// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "TPSProject.h"
#include "EnemyAnim.h"

#include <AIController.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/CapsuleComponent.h>

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

	// 월드에서 ATPSPlayer 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	target = Cast<ATPSPlayer>(actor);

	me = Cast<AEnemy>(GetOwner());

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

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
			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Idle"), true, true, FLinearColor::Green, 2.0f);
			break;

		case EEnemyState::Move:
			MoveState();
			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Move"), true, true, FLinearColor::Green, 2.0f);
			break;

		case EEnemyState::Attack:
			AttackState();
			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Attack"), true, true, FLinearColor::Black, 2.0f);
			break;

		case EEnemyState ::Damage:
			DamageState();
			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage"), true, true, FLinearColor::Green, 2.0f);
			break;

		case EEnemyState::Die:
			DieState();
			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Die"), true, true, FLinearColor::Green, 2.0f);
			break;

	}
	 

	// ...
}

void UEnemyFSM::IdleState()
{
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
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal());

	ai->MoveToLocation(destination);

	if (dir.Length() < attackRange)
	{
		mState = EEnemyState::Attack;

		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	// 목표 : 일정 시간에 한 번씩 공격하고 싶다.
	// 1. 시간이 흘러야 한다.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 공격 시간이 됐으니깝
	if (currentTime > attackDelayTime)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Attack!!!"), true, true, FLinearColor::Green, 2.0f);
		currentTime = 0;

		anim->bAttackPlay = true;
	}
	
	// 타깃과의 거리 체크
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	
	// 타깃과의 거리가 공격 범위를 벗어낫는지
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
	
	if (anim->isDead == false)
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Die false"), true, true, FLinearColor::Green, 2.0f);
	else
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Die true"), true, true, FLinearColor::Green, 2.0f);

	if (me->GetActorLocation().Z < -200.0f)
	{
		me->Destroy();
	}
	anim->animState = mState;
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;
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
	}
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{

	return false;
}

