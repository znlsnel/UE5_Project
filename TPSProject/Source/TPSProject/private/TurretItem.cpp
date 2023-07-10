// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretItem.h"
#include "TurretBullet.h"
#include "Enemy.h"
#include "EnemyFSM.h"


#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <particles/ParticleSystemComponent.h>
#include <NiagaraSystem.h>
#include <NiagaraFunctionLibrary.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>

ATurretItem::ATurretItem() : Super()
{
	Tags.Add("Turret");
	itemType = ItemType::Building;
	buildableItemType = BuildableItemType::Turret;

	enemySensorComp = CreateDefaultSubobject<USphereComponent>(TEXT("EnemySensor"));
	myCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myCylinder"));
	myMuzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMuzzle"));
	fireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));

	enemySensorComp->SetSphereRadius(sensorRange);
	enemySensorComp->SetupAttachment(boxCollision);
	enemySensorComp->AttachToComponent(boxCollision, FAttachmentTransformRules::KeepRelativeTransform);

	myCylinder->SetupAttachment(boxCollision);
	myCylinder->AttachToComponent(boxCollision, FAttachmentTransformRules::KeepRelativeTransform);

	myMuzzle->SetupAttachment(myCylinder);
	myMuzzle->AttachToComponent(myCylinder, FAttachmentTransformRules::KeepRelativeTransform);

	fireEffect->SetupAttachment(myMuzzle);
	fireEffect->AttachToComponent(myMuzzle, FAttachmentTransformRules::KeepRelativeTransform);

}

void ATurretItem::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	if (isTurretActive && currTarget) {
		FVector currPos = myCylinder->GetComponentLocation();
		FVector targetPos = currTarget->GetActorLocation();
		if (FVector::Distance(currPos, targetPos) > enemySensorComp->GetScaledSphereRadius()) {
			currTarget = nullptr;
			return;
		}
		FRotator LookVector = UKismetMathLibrary::FindLookAtRotation(currPos, targetPos);
		
		LookVector.Pitch = 0;
		LookVector.Roll = 0;

		FRotator currRot = myCylinder->GetComponentRotation();
		myCylinder->SetWorldRotation(FMath::RInterpTo(currRot, LookVector, DeltaSecond, 1.f));
		
		float offset = FMath::Abs(LookVector.Yaw - currRot.Yaw);
		isFireable = offset < 1;
	}
}

void ATurretItem::BeginPlay()
{
	Super::BeginPlay();
	fireEffect->Deactivate();
	FindEnemy();
}


void ATurretItem::FireLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(fireLoopTimer);
	
	if (isFireable && currTarget)
	{ // Attack Logic
		FHitResult hit;
		if (isShootingPossible(currTarget, hit) == false) {
			isFireable = false;
			isTurretActive = false;
			currTarget = nullptr;
			return;
		}
		else {

			CreateEffect(hit);

			if (hit.GetActor()->GetClass()->IsChildOf(AEnemy::StaticClass())) {
				AEnemy* tempEnemy = Cast<AEnemy>(hit.GetActor());
				tempEnemy->OnDamage(Damage, hit.BoneName, myPlayer);

			}
			UKismetSystemLibrary::PrintString(GetWorld(), hit.GetActor()->GetName());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());
		}
	}

	GetWorld()->GetTimerManager().SetTimer(fireLoopTimer, this, &ATurretItem::FireLoop, fireSpeed, false);
}

void ATurretItem::FindEnemy()
{
	GetWorldTimerManager().ClearTimer(findEnemyTimer);

	if (IsHidden() == false && isBuild == false && currTarget == nullptr){
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Finding. . ."));

		isTurretActive = true;

		TArray<AActor*> overlappingActors;
		enemySensorComp->GetOverlappingActors(overlappingActors);

		for (auto overlappingActor : overlappingActors) {
			if (overlappingActor->ActorHasTag("Enemy") == false)
				continue;

			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Find ! ! !"));

			FHitResult hit;
			AEnemy* enemy = Cast<AEnemy>(overlappingActor);
			if (enemy->isActive() && isShootingPossible(enemy, hit)) {
				currTarget = enemy;
				FireLoop();
			}

		}
	}

	GetWorldTimerManager().SetTimer(findEnemyTimer, this, &ATurretItem::FindEnemy, 1.f, false);
}

bool ATurretItem::isShootingPossible(AEnemy* enemy, FHitResult& hitResult)
{
	if (enemy->isDead())
		return false;

	FVector StartPos = myMuzzle->GetComponentLocation();
	FVector dir =  (enemy->GetActorLocation() - myMuzzle->GetComponentLocation()).GetSafeNormal();
	FVector EndPos = StartPos + dir * 2000;

	hitResult = LineTrace(StartPos, EndPos);

	if (hitResult.GetActor() && hitResult.GetActor()->GetClass()->IsChildOf(AEnemy::StaticClass()))
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

void ATurretItem::CreateEffect(FHitResult& hit)
{
	fireEffect->Activate(true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, myMuzzle->GetComponentLocation());

	UNiagaraComponent* tempTracer = UNiagaraFunctionLibrary::SpawnSystemAttached(bulletTracer, myMuzzle, TEXT(""), myMuzzle->GetComponentLocation(), FRotator(0), EAttachLocation::KeepWorldPosition, true);

	if (tempTracer)
	{
		tempTracer->SetNiagaraVariableBool(FString("User.Trigger"), true);

		if (hit.bBlockingHit == false)
			hit.ImpactPoint = hit.TraceEnd;

		TArray<FVector> TraceImpactPosArr;
		TraceImpactPosArr.Add(hit.ImpactPoint);


		UNiagaraDataInterfaceArrayFunctionLibrary::
			SetNiagaraArrayVector(tempTracer, TEXT("User.ImpactPositions"), TraceImpactPosArr);

		tempTracer->SetNiagaraVariablePosition(FString("User.MuzzlePostion"), myMuzzle->GetComponentLocation());
	}

	UNiagaraComponent* tempEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());

	if (tempEffect)
	{

		tempEffect->SetNiagaraVariableBool(TEXT("User.Trigger"), true);

		int32 surfacetype = 2;
		if (hit.GetActor() && hit.GetActor()->ActorHasTag(TEXT("Enemy"))) surfacetype = 1;

		tempEffect->SetNiagaraVariableInt(TEXT("User.SurfaceType"), surfacetype);

		TArray<FVector> ImpactPosArr;
		ImpactPosArr.Add(hit.ImpactPoint);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(tempEffect, TEXT("User.ImpactPositions"), ImpactPosArr);

		TArray<FVector> ImpactNormalArr;
		ImpactNormalArr.Add(hit.ImpactNormal);

		UNiagaraDataInterfaceArrayFunctionLibrary::
			SetNiagaraArrayVector(tempEffect, TEXT("User.ImpactNormals"), ImpactNormalArr);

		TArray<int32> tempInt32Array;
		tempInt32Array.Add(2);

		UNiagaraDataInterfaceArrayFunctionLibrary::
			SetNiagaraArrayInt32(tempEffect, TEXT("User.ImpactSurfaces"), tempInt32Array);

		tempEffect->SetNiagaraVariableInt(TEXT("User.NumberOfHits"), 1);
	}

}
