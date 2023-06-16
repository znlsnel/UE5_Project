// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretItem.h"
#include "TurretBullet.h"
#include "Enemy.h"
#include "EnemyFSM.h"


#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

ATurretItem::ATurretItem() : Super()
{
	itemType = ItemType::Building;
	buildableItemType = BuildableItemType::Turret;

	enemySensorComp = CreateDefaultSubobject<USphereComponent>(TEXT("EnemySensor"));
	enemySensorComp->SetSphereRadius(sensorRange);
	enemySensorComp->SetupAttachment(boxCollision);
	enemySensorComp->AttachToComponent(boxCollision, FAttachmentTransformRules::KeepRelativeTransform);
}

void ATurretItem::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
	if (GetWorldTimerManager().IsTimerActive(fireLoopTimer) == false)
	{
		TArray<AActor*> overlappnigActor;
		enemySensorComp->GetOverlappingActors(overlappnigActor);
		if (overlappnigActor.IsEmpty() == false)
			FireLoop();
	}
}

void ATurretItem::FireLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(fireLoopTimer);
	
	{ // Attack Logic
		GetFirePos();

		currTarget = FindEnemy();
		if (currTarget == nullptr)	{
			return;
		}

		FRotator tempRot =  UKismetMathLibrary::FindLookAtRotation(muzzlePos, currTarget->GetActorLocation());
		ATurretBullet* bullet = GetBullet();

		if (IsValid(bullet))
			bullet->InitBullet(muzzlePos, tempRot, myPlayer);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(fireLoopTimer, this, &ATurretItem::FireLoop, fireSpeed, false);
}

AEnemy* ATurretItem::FindEnemy()
{
	TArray<AActor*> overlappingActors;
	enemySensorComp->GetOverlappingActors(overlappingActors);

	for (auto overlappingActor : overlappingActors) {
		if (overlappingActor->ActorHasTag("Enemy") == false)
			continue;

		AEnemy* enemy = Cast<AEnemy>(overlappingActor);
		if (enemy->isActive() && isShootingPossible(enemy)) 
			return enemy;

	}
	return nullptr;
}

bool ATurretItem::isShootingPossible(AEnemy* enemy)
{
	FHitResult hit = LineTrace(muzzlePos, enemy->GetActorLocation());

	if (hit.GetActor() && hit.GetActor()->ActorHasTag("Enemy"))
		return true;
	else
		return false;
}

bool ATurretItem::isTargetsEmpty()
{
	TArray<AActor*> temp;
	enemySensorComp->GetOverlappingActors(temp);
	return temp.IsEmpty();
}

void ATurretItem::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < bulletCount; i++)
	{
		ATurretBullet* bullet =  Cast<ATurretBullet>(GetWorld()->SpawnActor(bulletFactory));
		
		bullet->SetActorHiddenInGame(true);
		bullets.Add(bullet);
	}
}

ATurretBullet* ATurretItem::GetBullet()
{
	for (auto bullet : bullets)
	{
		if (bullet->isFire == false) {
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Bullet!"));
			return bullet;
		}
		

	}

	return nullptr;
}
