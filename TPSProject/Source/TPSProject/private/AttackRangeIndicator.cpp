// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackRangeIndicator.h"
#include "Kwang.h"

#include <Components/DecalComponent.h>
#include <Components/SphereComponent.h>
#include <Components/PointLightComponent.h>
#include <Kismet/GameplayStatics.h>
#include <particles/ParticleSystemComponent.h>

// Sets default values
AAttackRangeIndicator::AAttackRangeIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	targetSensor = CreateDefaultSubobject<USphereComponent>(TEXT("Sensor"));
	decal = CreateDefaultSubobject<UDecalComponent>(TEXT("decal"));
	lightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	SkillEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillEffect"));

	decal->SetupAttachment(targetSensor);
	decal->AttachToComponent(targetSensor, FAttachmentTransformRules::KeepRelativeTransform);

	lightComponent->SetupAttachment(targetSensor);
	lightComponent->AttachToComponent(targetSensor, FAttachmentTransformRules::KeepRelativeTransform);

	SkillEffect->SetupAttachment(targetSensor);
	SkillEffect->AttachToComponent(targetSensor, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AAttackRangeIndicator::BeginPlay()
{
	Super::BeginPlay();
	SkillEffect->Deactivate();
}

// Called every frame
void AAttackRangeIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAttackRangeIndicator::SetActive(bool active, FVector pos, float setTime)
{
	if (active) {
		SetActorLocation(pos);
		SetActorHiddenInGame(false);
		isActiveDecal = true;

		GetWorldTimerManager().ClearTimer(EndTimer);
		GetWorldTimerManager().SetTimer(EndTimer, FTimerDelegate::CreateLambda(
			[&]() {
				SetActive(false);
			}), setTime, false);
	}
	else {
		isActiveDecal = false;
		SetActorHiddenInGame(true);
	}
}

void AAttackRangeIndicator::SetAttackTimer(AEnemy* enemy, float setTime)
{
	myEnemy = enemy;

	GetWorldTimerManager().ClearTimer(AttackTimer);
	GetWorldTimerManager().SetTimer(AttackTimer, FTimerDelegate::CreateLambda(
		[&]() {
			AttackToTargets();
			SkillEffect->Activate(true);
		}
	), setTime, false);
}

void AAttackRangeIndicator::SetMode(bool isMeleeMode)
{
	if (isMeleeMode) {
		SetActorScale3D(FVector(4));
		lightComponent->SetAttenuationRadius(900.f);
	}
	else {
		SetActorScale3D(FVector(2));
		lightComponent->SetAttenuationRadius(600.f);
	}
}

void AAttackRangeIndicator::AttackToTargets()
{
	if (myEnemy == nullptr)
		return;

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("AttackToTargets"));

	TArray<AActor*> actors;
	targetSensor->GetOverlappingActors(actors);
	myEnemy->OnAttackTargets(actors);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());

	GetWorldTimerManager().ClearTimer(EndTimer);
	GetWorldTimerManager().SetTimer(EndTimer, FTimerDelegate::CreateLambda(
		[&]() {
			SetActive(false);
		}), 1.2f, false);

}

