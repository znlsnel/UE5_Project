// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Gun.h"
#include "Enemy.h"
#include "EnemyFSM.h"

AWeapon_Gun::AWeapon_Gun()
{
	attachCharacterSocketName = FName("RightHandSocket");
}

void AWeapon_Gun::Attack()
{
	Super::Attack();
	if (currAmmo <= 0) return;
	FireWeapon();
}

void AWeapon_Gun::FireWeapon()
{
	TArray<FHitResult> pHitResultArr = LineTrace();

	for (auto HitResult : pHitResultArr)
	{
		createNiagara(HitResult);


	}

	if (currAmmo) {
		int randInt = FMath::RandRange(0, 100);
		if (randInt > myPlayer->abilityComp->GetSkillInfo(SkillType::LuckyShot)->powerValue)
			currAmmo--;
	}
}

void AWeapon_Gun::createNiagara(FHitResult pHitResult)
{
	UNiagaraComponent* tempTracer = UNiagaraFunctionLibrary::SpawnSystemAttached(TracerNS, weaponMeshComp, TEXT(""), weaponMeshComp->GetSocketLocation(TEXT("Muzzle")), FRotator(0), EAttachLocation::KeepWorldPosition, true);

	if (tempTracer)
	{
		tempTracer->SetNiagaraVariableBool(FString("User.Trigger"), true);
		//pHitResult.ImpactPoint = pHitResult.bBlockingHit ? pHitResult.ImpactPoint : pHitResult.TraceEnd;

		if (pHitResult.bBlockingHit == false)
			pHitResult.ImpactPoint = pHitResult.TraceEnd;



		TArray<FVector> TraceImpactPosArr;
		TraceImpactPosArr.Add(pHitResult.ImpactPoint);


		UNiagaraDataInterfaceArrayFunctionLibrary::
			SetNiagaraArrayVector(tempTracer, TEXT("User.ImpactPositions"), TraceImpactPosArr);

		tempTracer->SetNiagaraVariablePosition(FString("User.MuzzlePostion"), weaponMeshComp->GetSocketLocation("Muzzle"));

	}
	UNiagaraComponent* tempDecal = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactDecal, pHitResult.ImpactPoint, pHitResult.ImpactNormal.Rotation());

	CreateDecal(tempDecal, pHitResult);

	UNiagaraComponent* tempImpact = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, pHitResult.ImpactPoint, pHitResult.ImpactNormal.Rotation());

	CreateDecal(tempImpact, pHitResult);


	if (IsValid(pHitResult.GetActor()) && pHitResult.GetActor()->ActorHasTag(TEXT("Enemy")))
	{
		tempEnemy = Cast<AEnemy>(pHitResult.GetActor());

		int damage = weapDamage + myPlayer->abilityComp->GetSkillInfo(SkillType::gunProficiency)->powerValue;

		tempEnemy->fsm->OnDamageProcess(damage, myPlayer, pHitResult.BoneName);


		bool isDoubleAttack = false;
		int randInt = FMath::RandRange(1, 100);
		if (randInt < myPlayer->abilityComp->GetSkillInfo(SkillType::DoubleAttack)->powerValue)
			isDoubleAttack = true;

		if (isDoubleAttack) {
			GetWorldTimerManager().SetTimer(doubleAttackTimer, FTimerDelegate::CreateLambda([&]() {
				if (tempEnemy) {
					int damage = weapDamage + myPlayer->abilityComp->GetSkillInfo(SkillType::gunProficiency)->powerValue;

					tempEnemy->fsm->OnDamageProcess(damage, myPlayer, pHitResult.BoneName);
					tempEnemy = nullptr;
				}
				}), 0.1f, false);
		}
	}

}
