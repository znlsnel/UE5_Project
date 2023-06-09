// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBullet.h"
#include "Enemy.h"

#include <NiagaraComponent.h>
#include <NiagaraFunctionLibrary.h>
// Sets default values
ATurretBullet::ATurretBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ATurretBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (isFire)
		FireBullet(DeltaTime);
}

void ATurretBullet::FireBullet(float DeltaTime)
{
	FVector fv = GetActorForwardVector();
	AddActorLocalOffset(fv * bulletSpeed);
}

void ATurretBullet::InitBullet(FVector pos, FRotator rot)
{
	SetActorHiddenInGame(false);
	SetActorLocation(pos);
	SetActorRotation(rot);

	isFire = true;
}

void ATurretBullet::HitBullet(AActor* HitTarget)
{
	if (HitTarget->ActorHasTag("Enemy")) {
		DamageProcess(Cast<AEnemy>(HitTarget));
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionParticle, HitTarget->GetActorLocation(), GetActorRotation());

	isFire = false;
	SetActorHiddenInGame(true);
}

void ATurretBullet::DamageProcess(AEnemy* target)
{
	int damage = FMath::FloorToInt64(FMath::RandRange(attackPower * 0.8, attackPower * 1.2));

	target->OnDamage(damage);
	
}

