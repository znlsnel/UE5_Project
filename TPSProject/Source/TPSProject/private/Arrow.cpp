// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Enemy.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

#include <NiagaraComponent.h>
#include <NiagaraFunctionLibrary.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>

// Sets default values
AArrow::AArrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/BowsAndCrossbowsPack/Assets/Models/ArrowBolt/SM_Arrow.SM_Arrow'"));
	if (tempMesh.Succeeded())
		ArrowMesh->SetStaticMesh(tempMesh.Object);

	ArrowMesh->OnComponentHit.AddDynamic(this, &AArrow::OnHit);

	RootComponent = ArrowMesh;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMComp"));

	ArrowEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	ArrowEffect->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	SetActorHiddenInGame(true);
	ArrowEffect->SetHiddenInGame(true);
	ArrowEffect->SetupAttachment(ArrowMesh);

	ArrowMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();


}

void AArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsUsed == false) return;

	if (OtherActor->ActorHasTag(FName("Enemy")))
	{
		Cast<AEnemy>(OtherActor)->OnDamage(attackDamage);
		// TODO 데미지
		// Blood Effect
	}
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("OnHit!"));
	// Actor에 SetAttack~~ 해버리기 
	ArrowEffect->SetHiddenInGame(true);
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UNiagaraComponent* tempImpact = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

	FHitResult k = Hit;
	CreateDecal(tempImpact, k);

	ReturnObject(0.f);
}

void AArrow::ReturnObject(float returnTime)
{
	GetWorld()->GetTimerManager().ClearTimer(returnObjectTimer);

	GetWorld()->GetTimerManager().SetTimer(returnObjectTimer, FTimerDelegate::CreateLambda([&]()
		{
			CancelArrow();
		}), returnTime, false);
}

void AArrow::InitArrow(FVector ArrowHeadSocketPos)
{
	SetActorHiddenInGame(false);
	

	FVector pos = GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(pos, ArrowHeadSocketPos));
	bIsUsed = true;
	AddActorLocalRotation(FRotator(-90, 0, 0));
	ArrowEffect->SetHiddenInGame(true);
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

bool AArrow::ShootArrow(FVector target, float power)
{
	if (power < 0.4f){
		CancelArrow();
		return false;
	}
	FVector pos = GetActorLocation();
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(pos, target);
	SetActorRotation(rot);
	AddActorLocalRotation(FRotator(-90, 0, 0));

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("power : %f"), power));

	power /= 1.5f;

	ProjectileMovementComp->SetUpdatedComponent(ArrowMesh);
	ProjectileMovementComp->InitialSpeed = 7000.0f * power;
	ProjectileMovementComp->MaxSpeed = 7000.0f * power;
	//ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.3f;
	ProjectileMovementComp->Velocity = rot.Vector() * ProjectileMovementComp->InitialSpeed;

	ArrowEffect->ReinitializeSystem();
	ArrowEffect->SetHiddenInGame(false);
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	ReturnObject(10.0f);
	return true;
}

void AArrow::CreateDecal(UNiagaraComponent* tempDecal, FHitResult& pHitResult)
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


void AArrow::CancelArrow()
{
	bIsUsed = false;
	SetActorHiddenInGame(true);
	ProjectileMovementComp->SetUpdatedComponent(NULL);
	ArrowEffect->SetHiddenInGame(true);
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

