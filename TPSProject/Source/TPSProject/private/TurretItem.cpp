// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretItem.h"
#include "TurretBullet.h"
#include "Enemy.h"
#include <Components/SphereComponent.h>
#include <Kismet/KismetMathLibrary.h>

ATurretItem::ATurretItem()
{
	itemType = ItemType::Building;
	buildableItemType = BuildableItemType::Turret;
}

void ATurretItem::FireWeapon(AEnemy* target, bool bOverlapStart)
{
	if (isBuild) return;

	if (bOverlapStart) {
		Targets.Add(target, targetId++);
		if (GetWorldTimerManager().IsTimerActive(fireLoopTimer) == false)
			FireLoop();
	}
	else {
		Targets.FindAndRemoveChecked(target);

		if (Targets.Num() <= 0)
			GetWorld()->GetTimerManager().ClearTimer(fireLoopTimer);
	}
}

void ATurretItem::FireLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(fireLoopTimer);
	
	{ // Attack Logic
		if (currTarget == nullptr)
			currTarget = Targets.begin().Key();
		
		GetFirePos();

		FRotator tempRot =  UKismetMathLibrary::FindLookAtRotation(muzzlePos, currTarget->GetActorLocation());
		ATurretBullet* bullet = GetBullet();

		if (IsValid(bullet))
			bullet->InitBullet(muzzlePos, tempRot);
	}

	GetWorld()->GetTimerManager().SetTimer(fireLoopTimer, this, &ATurretItem::FireLoop, fireSpeed, true);
}

bool ATurretItem::isTargetsEmpty()
{
	return Targets.IsEmpty();
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
		if (bullet->isFire == false)
			return bullet;
	}
	return nullptr;
}
