// Fill out your copyright notice in the Description page of Project Settings.


#include "RiktorThunder.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Doomstone.h"

#include <AIController.h>
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <particles/ParticleSystemComponent.h>

// Sets default values
ARiktorThunder::ARiktorThunder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	playerSensor = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerSensor"));
	mainEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MainEffect"));
	AttackEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AttackEffect"));

	playerSensor->SetupAttachment(GetMesh());
	playerSensor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	mainEffect->SetupAttachment(GetMesh());
	mainEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	AttackEffect->SetupAttachment(GetMesh());
	AttackEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts or when spawned
void ARiktorThunder::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	
	playerSensor->OnComponentBeginOverlap.AddDynamic(this, &ARiktorThunder::OverlapBegin);


	myAI = Cast<AAIController>(GetController());
}

// Called every frame
void ARiktorThunder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARiktorThunder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARiktorThunder::SetActiveThunder(bool active, AEnemy* parent,  FVector GenPos, AActor* targetActor)
{
	if (active) {
		myParent = parent;
		target = targetActor;

		SetActorLocation(GenPos);
		SetActorHiddenInGame(false);
		mainEffect->Activate(true);
		isActiveThunder = true;

		MoveLoop();
		GetWorldTimerManager().ClearTimer(EndTimer);
		GetWorldTimerManager().SetTimer(EndTimer, FTimerDelegate::CreateLambda(
			[&]() {
				SetActiveThunder(false);
			}), 10.f, false);
	}
	else {
		SetActorHiddenInGame(true);
		mainEffect->Deactivate();
		AttackEffect->Deactivate();
		isActiveThunder = false;
		myAI->StopMovement();
	}
}

void ARiktorThunder::MoveLoop()
{
	GetWorldTimerManager().ClearTimer(AIMoveLoopTimer);
	if (isActiveThunder == false)
		return;

	if (target) {
		myAI->MoveToActor(target);
	}
	if (myParent->fsm->anim->isDead) {
		SetActiveThunder(false);
	}

	GetWorldTimerManager().SetTimer(AIMoveLoopTimer, this, &ARiktorThunder::MoveLoop, 1.f, false);
}

void ARiktorThunder::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isActiveThunder == false)
		return;

	if (OtherActor->ActorHasTag("Player")) {
		isActiveThunder = false;
		AttackEffect->Activate(true);
		mainEffect->Deactivate();

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
		Cast<ATPSPlayer>(OtherActor)->OnHitEvent(3, GetActorLocation());


		GetWorldTimerManager().ClearTimer(EndTimer);
		GetWorldTimerManager().SetTimer(EndTimer, FTimerDelegate::CreateLambda(
			[&]() {
				SetActiveThunder(false);
			}), 2.f, false);
	}
}

