// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "TPSProject.h"
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

	// ���忡�� ATPSPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	target = Cast<ATPSPlayer>(actor);

	me = Cast<AEnemy>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	switch (mState)
	{
		case EEnemyState::Idle:
			IdleState();
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Idle"), true, true, FLinearColor::Green, 2.0f);
			break;

		case EEnemyState::Move:
			MoveState();
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Move"), true, true, FLinearColor::Green, 2.0f);
			break;

		case EEnemyState::Attack:
			AttackState();
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Attack"), true, true, FLinearColor::Black, 2.0f);
			break;

		case EEnemyState ::Damage:
			DamageState();
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage"), true, true, FLinearColor::Green, 2.0f);
			break;

		case EEnemyState::Die:
			DieState();
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Die"), true, true, FLinearColor::Green, 2.0f);
			break;

	}
	 

	// ...
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
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());

	if (dir.Length() < attackRange)
	{
		mState = EEnemyState::Attack;
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
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Attack!!!"), true, true, FLinearColor::Green, 2.0f);
		currentTime = 0;
	}
	
	// Ÿ����� �Ÿ� üũ
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	
	// Ÿ����� �Ÿ��� ���� ������ �������
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
	}
}

void UEnemyFSM::DieState()
{
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
		mState = EEnemyState::Damage;
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

