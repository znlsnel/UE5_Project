// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Inventory.h"

#include <Kismet/KismetSystemLibrary.h>
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

}

void AWeapon::UnSynchronizeWhitPlayer()
{
	//DetachRootComponentFromParent();
	UInventory* inventory = myPlayer->GetInventory();

	bool bItemAdded = inventory->AddItemToInventory(this);
	
	if (bItemAdded == false)
		DropItemOnGround();

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
	if (anim) 
		anim->PlayPlayerMontage(CharacterFireAM);


	GetWorld()->GetFirstPlayerController()-> PlayerCameraManager->StartCameraShake(FireCamShakeClass);

	if (!FireCamShakeClass)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("No FireCamShakeClass")));
	}

	FHitResult pHitResult = LineTrace(); 

	UNiagaraComponent* tempTracer =  UNiagaraFunctionLibrary::SpawnSystemAttached(TracerNS, weaponMeshComp, TEXT(""), weaponMeshComp->GetSocketLocation(TEXT("Muzzle")), FRotator(0, 0, 0), EAttachLocation::KeepWorldPosition, true);


	if (tempTracer)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Tracer")));
		tempTracer->SetNiagaraVariableBool(FString("User.Trigger"), true);

		FVector tempPos = pHitResult.bBlockingHit ? pHitResult.ImpactPoint : pHitResult.TraceEnd;
		TArray<FVector> TraceImpactPosArr;
		TraceImpactPosArr.Add(tempPos);

		UNiagaraDataInterfaceArrayFunctionLibrary::
			SetNiagaraArrayVector(tempTracer, TEXT("User.ImpactPositions"), TraceImpactPosArr);

		tempTracer->SetNiagaraVariablePosition(FString("User.MuzzlePostion"), weaponMeshComp->GetSocketLocation("Muzzle"));
			
	}

	if (currAmmo) currAmmo--;
}

// Trace From Camera
FHitResult AWeapon::LineTrace()
{
	FVector TraceStartPoint;
	FRotator TraceStartRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, OUT TraceStartRotation);

	float traceLength = 5000.f;
	FVector LineTraceEnd = TraceStartPoint + TraceStartRotation.Vector() * traceLength;

	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

	if (isHit)
		LineTraceEnd = pHitResult.ImpactPoint;

	TraceStartPoint = weaponMeshComp->GetSocketLocation("Muzzle");

	isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

	
	return pHitResult;
}

void AWeapon::HideWeapon()
{
	weaponMeshComp->SetVisibility(false);

}

void AWeapon::UncoverWeapon()
{
	weaponMeshComp->SetVisibility(true);
	if (anim)
	{
		anim->PlayPlayerMontage(CharacterEquipAM);
	}
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
	int capacity = FMath::Min(FMath::Min(MagazineSize - currAmmo, Ammo), MagazineSize);

	currAmmo += capacity;
	Ammo -= capacity;

	if (anim)
		anim->PlayPlayerMontage(CharacterReloadAM);
	weaponMeshComp->PlayAnimation(WeaponReloadAnim, false);

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
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

