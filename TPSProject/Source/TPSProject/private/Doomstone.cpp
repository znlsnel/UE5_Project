// Fill out your copyright notice in the Description page of Project Settings.


#include "Doomstone.h"
#include "Enemy.h"
#include "TPSPlayer.h"

#include <Components/BoxComponent.h>
#include <Camera/CameraComponent.h>
#include <Camera/CameraActor.h>
#include <Camera/PlayerCameraManager.h>
#include <Kismet/GameplayStatics.h>
#include <GeometryCollection/GeometryCollectionComponent.h>
#include <GeometryCollection/GeometryCollection.h>
#include <GeometryCollection/GeometryCollectionActor.h>

// Sets default values
ADoomstone::ADoomstone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	statueMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("statueMesh"));

	statueMesh->SetupAttachment(boxCollision);
	statueMesh->AttachToComponent(boxCollision, FAttachmentTransformRules::KeepRelativeTransform);
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("DoomStone");
	Tags.Add("EnemysTarget");
}

void ADoomstone::DestoryStatue()
{
	if (statueActor)
		statueActor->GeometryCollectionComponent->SetSimulatePhysics(true);

	TArray<AActor*> enemys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemys);
	for (auto enemy : enemys) {
		Cast<AEnemy>(enemy)->StopMove();
	}
}

// Called when the game starts or when spawned
void ADoomstone::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("DoomStone");
	Tags.Add("EnemysTarget");
	statueMesh->SetHiddenInGame(true);
	MakeStatueActor();

	Hp = MaxHp;
//	hpWidget->parent = this;
}

// Called every frame
void ADoomstone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#include <Kismet/KismetSystemLibrary.h>

void ADoomstone::OnHitEvent(int Damage)
{
	if (Hp <= 0) return;

	Hp = FMath::Max(Hp - Damage, 0);

	HpPercent = (float)Hp / (float)MaxHp;

	if (Hp == 0) {
		DestoryStatue();
		isDestory = true;
		Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GameOver();
	}

}

void ADoomstone::MakeStatueActor()
{
	if (statueActor)
		statueActor->Destroy();
	isDestory = false;
	statueActor = Cast<AGeometryCollectionActor>(GetWorld()->SpawnActor(statueActorFactory));
	statueActor->GeometryCollectionComponent->SetSimulatePhysics(false);

	FVector loc = GetActorLocation();
	loc.Z = -20;
	statueActor->SetActorLocation(loc);
	statueActor->SetActorRotation(GetActorRotation());
}


