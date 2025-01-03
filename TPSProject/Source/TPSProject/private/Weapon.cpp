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

	SetActorLocation(myPlayer->GetMesh()->GetSocketLocation(attachCharacterSocketName));

	AttachToComponent(myPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform,attachCharacterSocketName);

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
		DropItemOnGround();

	SetSync(false);
}

void AWeapon::Attack()
{
	if (isSynchronized == false) return;
	if (currAmmo <= 0)
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
	myPlayer->PlayMontage(CharacterFireAM);


	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(FireCamShakeClass);

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
		currFireSpread = FMath::Min(currFireSpread * 1.1f, FireSpread * 1.5f);
		lastFiredTime = GetWorld()->GetRealTimeSeconds();
	}
	else
	{
		currFireSpread = FireSpread;
		lastFiredTime = 0.f;
	}



}

// Trace From Camera
TArray<FHitResult> AWeapon::LineTrace()
{
	APlayerCameraManager* tempCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector TraceStartPoint = tempCamera->GetCameraLocation();
	FRotator TraceStartRotation = tempCamera->GetCameraRotation();

	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, OUT TraceStartRotation);



	float traceLength = 10000.f;
	FVector LineTraceEnd = TraceStartPoint + TraceStartRotation.Vector() * traceLength;

	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

	TraceStartPoint = weaponMeshComp->GetSocketLocation("Muzzle");

	if (pHitResult.bBlockingHit == false)
		pHitResult.ImpactPoint = LineTraceEnd;

	TArray<FHitResult> hitArr;
	for (int i = 0; i < FireBulletCount; i++)
	{
		LineTraceEnd = TraceStartPoint + (UKismetMathLibrary::RandomUnitVectorInConeInDegrees(MyNormalize(pHitResult.ImpactPoint - TraceStartPoint), currFireSpread) * traceLength);

		isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

		if (pHitResult.bBlockingHit == false)
			pHitResult.ImpactPoint = LineTraceEnd;

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
	AWeapon* tempWeapon = myPlayer->playerFire->currWeapon;

	if (IsValid(tempWeapon) && tempWeapon->weaponType == weaponType)
	{
		tempWeapon->UnSynchronizeWhitPlayer();
	}
	//myPlayer->playerFire->currWeapon = this;
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
	myPlayer->PlayMontage(CharacterReloadAM);

	weaponMeshComp->PlayAnimation(WeaponReloadAnim, false);

}




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
	InitFireSpread = FireSpread;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

