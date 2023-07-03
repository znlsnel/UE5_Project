// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include "EnemyManager.h"
#include "DamageUI.h"
#include "DamageUIActor.h"
#include "RoundUI.h"
#include "TPSPlayer.h"
#include "EnemyHpBar.h"
#include "DamageWidget.h"

#include <Components/BoxComponent.h>
#include <Components/AudioComponent.h>
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
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Enemy"));

	targetSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("targetSensor"));
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	damageEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamageEffect"));

	// 1. 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/ParagonRiktor/Characters/Heroes/Riktor/Meshes/Riktor.Riktor'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> tempParticle(TEXT("ParticleSystem'/Game/ParagonSparrow/FX/Particles/Sparrow/Skins/Rogue/P_Sparrow_UltHitWorld.P_Sparrow_UltHitWorld'"));

	if (tempParticle.Succeeded())
	{
		damageEffect->Template = tempParticle.Object;
	}


	// 월드에 배치되거나 스폰될 때 자동으로
	// AIController부터 Possess될 수 있도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	myController = GetController();

	targetSensor->SetupAttachment(GetMesh());
	targetSensor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	damageEffect->SetupAttachment(GetMesh());
	damageEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	hpBar = CreateWidget<UEnemyHpBar>(GetWorld(), hpBarFactory);
	hpBar->AddToViewport();
	damageEffect->Deactivate();
	targetSensor->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
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




void AEnemy::OnDamage(int damage, FName boneName, class ATPSPlayer* AttackPlayer, bool ignoreHpEvent)
{
	fsm->OnDamageProcess(damage, AttackPlayer, boneName, ignoreHpEvent);
}


bool AEnemy::isActive()
{
	return fsm->isActive;
}

void AEnemy::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("DoomStone") || OtherActor->ActorHasTag("Player")) {
		if (fsm->isInAttackRange == false) {
			fsm->anim->PlayAttackAnim(false, true);
			fsm->isInAttackRange = true;
		}
	}
	else if (OtherActor->ActorHasTag("Barricade")) {
		if (fsm->isPossibleToMove() == false || fsm->isInAttackRange == false) {
			fsm->anim->PlayAttackAnim(false, true);
			fsm->isInAttackRange = true;
		}
	}
}

bool AEnemy::isOverlapingTargets()
{
	TArray<AActor*> actors;
	targetSensor->GetOverlappingActors(actors);

	for (auto actor : actors) {
		if (actor->ActorHasTag("DoomStone") || actor->ActorHasTag("Player") || actor->ActorHasTag("Barricade")) {
			return true;
		}
	}

	return false; 
}

void AEnemy::OnAttackTargets(TArray<AActor*> actors)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("OnAttackTargets"));
	fsm->anim->AttackToTargets(actors, fsm->anim->AttackDamage * 2);
}


