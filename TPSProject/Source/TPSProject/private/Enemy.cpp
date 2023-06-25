// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyManager.h"
#include "DamageUI.h"
#include "DamageUIActor.h"
#include "RoundUI.h"
#include "TPSPlayer.h"
#include "EnemyHpBar.h"
#include "DamageWidget.h"

#include <Components/WidgetComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include <Camera/CameraComponent.h>

// Sets default values
AEnemy::AEnemy()
{
	//bReplicates = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Enemy"));

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// 1. 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/ParagonRiktor/Characters/Heroes/Riktor/Meshes/Riktor.Riktor'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeScale3D(FVector(0.88));
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	// 월드에 배치되거나 스폰될 때 자동으로
	// AIController부터 Possess될 수 있도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	myController = GetController();
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	hpBar = CreateWidget<UEnemyHpBar>(GetWorld(), hpBarFactory);
	hpBar->AddToViewport();
}
#include <Kismet/KismetMathLibrary.h>
// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AddWorldDamageUI(int Damage, float currHpPercent, float preHpPercent)
{

	if (player == nullptr)
		player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FVector pos = GetActorLocation();
	pos.Z += (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2) + 30;


	UDamageWidget* tempWidget = player->GetDamageWidget();
	if (IsValid(tempWidget)) {
		tempWidget->InitDamageWidget(pos, Damage);
	}
	hpBar->InitHpWidget(this, currHpPercent, preHpPercent);
}




void AEnemy::DieEvent(ATPSPlayer* AttackPlayer)
{

}

void AEnemy::OnDamage(int damage, FName boneName, class ATPSPlayer* AttackPlayer)
{
	fsm->OnDamageProcess(damage, AttackPlayer, boneName);
}

void AEnemy::SetTarget(AActor* target)
{
	fsm->SetTarget(target);
}

bool AEnemy::isActive()
{
	return fsm->isActive;
}

