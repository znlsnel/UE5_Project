// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBullet.h"
#include "Enemy.h"

#include <NiagaraComponent.h>
#include <Components/SphereComponent.h>
#include <NiagaraFunctionLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
// Sets default values
ATurretBullet::ATurretBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	enemySensorComp = CreateDefaultSubobject<USphereComponent>(TEXT("EnemySensor"));
	enemySensorComp->SetSphereRadius(damageRange);
}

// Called when the game starts or when spawned
void ATurretBullet::BeginPlay()
{
	enemySensorComp->OnComponentBeginOverlap.AddDynamic(this, &ATurretBullet::BegineOverlapEvent);
	enemySensorComp->OnComponentEndOverlap.AddDynamic(this, &ATurretBullet::EndOverlapEvent);
	
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
	AddActorWorldOffset(FireRot.Vector() * bulletSpeed * DeltaTime);
	if (lastFireTime == 0.f)
		lastFireTime = GetWorld()->GetTimeSeconds();

	else if (GetWorld()->GetTimeSeconds() - lastFireTime > 3.f){
		lastFireTime = 0.f;

		isFire = false;
		SetActorHiddenInGame(true);
	}

}

void ATurretBullet::InitBullet(FVector pos, FRotator rot)
{
	SetActorHiddenInGame(false);
	SetActorLocation(pos);
	SetActorRotation(rot);

	FireRot = rot;
	isFire = true;
}

void ATurretBullet::HitBullet(FHitResult hit)
{
	if (isFire == false) return;

	DamageProcess();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionParticle, hit.ImpactPoint, hit.ImpactNormal.Rotation());

	lastFireTime = 0.f;
	isFire = false;
	SetActorHiddenInGame(true);
}

void ATurretBullet::DamageProcess()
{
	GetWorldTimerManager().SetTimer(damageTimer, FTimerDelegate::CreateLambda(
		[&]() {
			TArray<AActor*> OutOverlappingActors;
			enemySensorComp->GetOverlappingActors(OutOverlappingActors);

			if (OutOverlappingActors.IsEmpty()) {
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("OutOverlappingActors Empty!!"));
				return;
			}
			
			int damage = FMath::FloorToInt64(FMath::RandRange(attackPower * 0.8, attackPower * 1.2));

			for (auto overlappingActor : OutOverlappingActors)
			{
				if (overlappingActor->ActorHasTag(TEXT("Enemy")) == false) {
					UKismetSystemLibrary::PrintString(GetWorld(), TEXT(" No Enemy !!"));

					return;
				}
				
				AEnemy* enemy = Cast<AEnemy>(overlappingActor);
				float distance = (enemy->GetActorLocation() - GetActorLocation()).Length();

				if (distance < 100) {
				}
				else if (distance < 400)
					damage *= 0.7;
				else
					damage *= 0.4;
					
				enemy->OnDamage(damage);
			}
		}
	), 0.2f, false);
	
}

void ATurretBullet::BegineOverlapEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATurretBullet::EndOverlapEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

