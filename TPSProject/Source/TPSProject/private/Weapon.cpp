// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

#include <Kismet/KismetSystemLibrary.h>

void AWeapon::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	myPlayer = player;
	SetActorLocation(myPlayer->GetMesh()->GetSocketLocation("hand_rSocket"));
	AttachToComponent(myPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
	SetActorRotation(FRotator(0, -90, 0));
	anim = Cast<UPlayerAnim>(myPlayer->GetMesh()->GetAnimInstance());
	isSynchronized = true;
}

void AWeapon::Attack()
{
	if (isSynchronized == false) return;

	weaponMeshComp->PlayAnimation(WeaponFireAnim, false);
	if (anim) 
		anim->PlayAttackAnim();
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("No anim")));
	}

	GetWorld()->GetFirstPlayerController()-> PlayerCameraManager->StartCameraShake(FireCamShakeClass);

	if (!FireCamShakeClass)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("No FireCamShakeClass")));
	}

	FHitResult pHitResult = LineTrace(); 

	UNiagaraComponent* tempTracer =  UNiagaraFunctionLibrary::SpawnSystemAttached(TracerNS, weaponMeshComp, TEXT(""), weaponMeshComp->GetSocketLocation(TEXT("Muzzle")), FRotator(0, 0, 0), EAttachLocation::KeepWorldPosition, true);

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
}

// Trace From Camera
FHitResult AWeapon::LineTrace()
{
	FVector TraceStartPoint;
	FRotator TraceStartRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, TraceStartRotation);

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
}

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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

