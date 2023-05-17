// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Inventory.h"
#include "Enemy.h"
#include "EnemyFSM.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

void AWeapon::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	myPlayer = player;
	SetActorHiddenInGame(false);
	DiscardWeaponIfAlreadyExists();
	RemovePickupCollision();

	SetActorLocation(myPlayer->GetMesh()->GetSocketLocation("hand_rSocket"));

	AttachToComponent(myPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));

	SetActorRelativeRotation(FRotator(0, 90, 0));
	anim = Cast<UPlayerAnim>(myPlayer->GetMesh()->GetAnimInstance());
	isSynchronized = true;
	currAmmo = FMath::Min(Ammo, MagazineSize);

	SetSync(true);
}

void AWeapon::UnSynchronizeWhitPlayer()
{

	UInventory* inventory = myPlayer->GetInventory();

	bool bItemAdded = true;
	inventory->AddItemToInventory(this);


	if (bItemAdded == false)
		myPlayer->DropItemInServer(this);

	SetSync(false);
}

void AWeapon::Attack()
{

	if (isSynchronized == false) return;
	if (currAmmo == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, weaponDryClick, myPlayer->GetActorLocation());
		return;
	}

	weaponMeshComp->PlayAnimation(WeaponFireAnim, false);
	//if (anim)
	//{
	//	anim->currMontage = CharacterFireAM;
	//	anim->PlayMontage(CharacterFireAM);
	//}
	myPlayer->PlayMontageInServer(CharacterFireAM);


	GetWorld()->GetFirstPlayerController()-> PlayerCameraManager->StartCameraShake(FireCamShakeClass);

	if (!FireCamShakeClass)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("No FireCamShakeClass")));
	}

	if (lastFiredTime == 0.f)
	{
		lastFiredTime = GetWorld()->GetRealTimeSeconds();
	}
	else if (GetWorld()->GetRealTimeSeconds() - lastFiredTime < 1.2)
	{
		currFireSpread = FMath::Min(currFireSpread* 1.1f, FireSpread * 1.5f );
		lastFiredTime = GetWorld()->GetRealTimeSeconds();
	}
	else
	{
		currFireSpread = FireSpread;
		lastFiredTime = 0.f;
	}

	FireWeapon();

}

// Trace From Camera
TArray<FHitResult> AWeapon::LineTrace()
{
	FVector TraceStartPoint;
	FRotator TraceStartRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, OUT TraceStartRotation);

	float traceLength = 10000.f;
	FVector LineTraceEnd = TraceStartPoint + TraceStartRotation.Vector() * traceLength;

	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

	TraceStartPoint = weaponMeshComp->GetSocketLocation("Muzzle");


	TArray<FHitResult> hitArr;

	

	for (int i = 0; i < FireBulletCount; i++)
	{
		LineTraceEnd =  TraceStartPoint + (UKismetMathLibrary::RandomUnitVectorInConeInDegrees(MyNormalize(pHitResult.ImpactPoint - TraceStartPoint), currFireSpread) * traceLength);

		isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

		hitArr.Add(pHitResult);
		
	}
	return hitArr;
}

void AWeapon::HideWeapon()
{
	//weaponMeshComp->SetVisibility(false);
	SetActorHiddenInGame(true);
}

void AWeapon::UncoverWeapon()
{
	//weaponMeshComp->SetVisibility(true);
	SetActorHiddenInGame(false);
}

void AWeapon::DiscardWeaponIfAlreadyExists()
{
	AWeapon* tempWeapon = myPlayer->playerFire->primaryWeapon;

	tempWeapon = weaponSlotType == WeaponSlotType::PrimarySlot ? myPlayer->playerFire->primaryWeapon : myPlayer->playerFire->secondaryWeapon;
	
	if (tempWeapon != nullptr && tempWeapon != this)
	{
		tempWeapon->UnSynchronizeWhitPlayer();
	}

	switch (weaponSlotType)
	{
	case WeaponSlotType::PrimarySlot:
		myPlayer->playerFire->primaryWeapon = this;
		myPlayer->playerFire->EquipPrimaryWeapon();
		break;
	case WeaponSlotType::SecondarySlot:
		myPlayer->playerFire->secondaryWeapon = this;
		myPlayer->playerFire->EquipSecondaryWeapon();
		break;
	}
}


void AWeapon::Reload()
{
	if (Ammo == 0) return;
	if (currAmmo == MagazineSize) return;
	int capacity = FMath::Min(FMath::Min(MagazineSize - currAmmo, Ammo), MagazineSize);

	currAmmo += capacity;
	Ammo -= capacity;

	//if (anim)
	//	anim->PlayMontage(CharacterReloadAM);
	myPlayer->PlayMontageInServer(CharacterReloadAM);

	weaponMeshComp->PlayAnimation(WeaponReloadAnim, false);

}


void AWeapon::FireWeapon()
{
	TArray<FHitResult> pHitResultArr = LineTrace();

	for (auto HitResult : pHitResultArr)
	{
		myPlayer->createNiagara(HitResult);


	}



	if (currAmmo) currAmmo--;
}

//void AWeapon::createNiagara_Implementation(FHitResult pHitResult)
//{
//	MulticastNiaga(pHitResult);
//}

void AWeapon::createNiagara(FHitResult pHitResult)
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


		enemy->fsm->OnDamageProcess(damage, myPlayer);

	}
}

//void AWeapon::ClickWidget_Implementation(bool isFire)
//{
//	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ClickWidget! in Server"));
//	ClickWorldWidget(isFire);
//	//ClickWidgetMulti(isFire);
//}
//
//void AWeapon::ClickWidgetMulti_Implementation(bool isFire)
//{
//	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ClickWidget! in Multi"));
//	ClickWorldWidget(isFire);
//}

FVector AWeapon::MyNormalize(const FVector& Invec)
{
	double Length = Invec.Size();

	if (Length > 0.f)
		return Invec / Length;
	return FVector::ZeroVector;
}

void AWeapon::CreateDecal(UNiagaraComponent* tempDecal, FHitResult& pHitResult)
{
	if (tempDecal)
	{

		tempDecal->SetNiagaraVariableBool(TEXT("User.Trigger"), true);

		int32 surfacetype = 2;
		if (pHitResult.GetActor() && pHitResult.GetActor()->ActorHasTag(TEXT("Enemy"))) surfacetype = 1;

		tempDecal->SetNiagaraVariableInt(TEXT("User.SurfaceType"), surfacetype);

		TArray<FVector> ImpactPosArr;
		ImpactPosArr.Add(pHitResult.ImpactPoint);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(tempDecal, TEXT("User.ImpactPositions"), ImpactPosArr);

		TArray<FVector> ImpactNormalArr;
		ImpactNormalArr.Add(pHitResult.ImpactNormal);

		UNiagaraDataInterfaceArrayFunctionLibrary::
			SetNiagaraArrayVector(tempDecal, TEXT("User.ImpactNormals"), ImpactNormalArr);

		TArray<int32> tempInt32Array;
		tempInt32Array.Add(2);

		UNiagaraDataInterfaceArrayFunctionLibrary::
			SetNiagaraArrayInt32(tempDecal, TEXT("User.ImpactSurfaces"), tempInt32Array);

		tempDecal->SetNiagaraVariableInt(TEXT("User.NumberOfHits"), 1);
	}
}


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Weapon"));

	itemType = ItemType::Weapon;
	//CreatePickupCollision();

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	currFireSpread = FireSpread;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

