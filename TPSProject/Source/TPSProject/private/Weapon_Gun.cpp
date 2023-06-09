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
		myPlayer->createNiagara(HitResult);


	}

	if (currAmmo) currAmmo--;
}

void AWeapon_Gun::createNiagara(FHitResult pHitResult)
{
	UNiagaraComponent* tempTracer = UNiagaraFunctionLibrary::SpawnSystemAttached(TracerNS, weaponMeshComp, TEXT(""), weaponMeshComp->GetSocketLocation(TEXT("Muzzle")), FRotator(0), EAttachLocation::KeepWorldPosition, true);



	if (tempTracer)
	{
		tempTracer->SetNiagaraVariableBool(FString("User.Trigger"), true);

		FVector tempPos = pHitResult.bBlockingHit ? pHitResult.ImpactPoint : pHitResult.TraceEnd;
		TArray<FVector> TraceImpactPosArr;
		TraceImpactPosArr.Add(tempPos);

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
		AEnemy* enemy = Cast<AEnemy>(pHitResult.GetActor());

		int damage = weapDamage * (myPlayer->AdditionalAttackPower + 1);

		FName tempName = pHitResult.BoneName;
		if (tempName == FName("head"))
			damage *= 2;

		else if (tempName == FName("spine_03"))
			damage *= 1;

		else
			damage = damage - (damage / 4);

		enemy->fsm->SetTarget(myPlayer);
		enemy->fsm->OnDamageProcess(damage, myPlayer);
	}

}
