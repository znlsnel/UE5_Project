// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "Enemy.h"
#include "TPSPlayer.h"


#include <particles/ParticleSystemComponent.h>
#include <Components/DecalComponent.h>
#include <Components/SphereComponent.h>
#include <Components/AudioComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PointLightComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ASkill::ASkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	enemySensor = CreateDefaultSubobject<USphereComponent>(TEXT("sensor"));
	SkillEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillEffect"));
	SkillPreview = CreateDefaultSubobject<UDecalComponent>(TEXT("SkillPreview"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	previewLine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewLine"));

	SkillEffect->SetupAttachment(enemySensor);
	SkillEffect->AttachToComponent(enemySensor, FAttachmentTransformRules::KeepRelativeTransform);

	SkillPreview->SetupAttachment(enemySensor);
	SkillPreview->AttachToComponent(enemySensor, FAttachmentTransformRules::KeepRelativeTransform);

	previewLine->SetupAttachment(SkillPreview);
	previewLine->AttachToComponent(SkillPreview, FAttachmentTransformRules::KeepRelativeTransform);

	Light->SetupAttachment(SkillPreview);
	Light->AttachToComponent(SkillPreview, FAttachmentTransformRules::KeepRelativeTransform);


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded()) {
		previewLine->SetStaticMesh(tempMesh.Object);
	}
	previewLine->SetRelativeScale3D(FVector(0.05, 0.05, 30));
	previewLine->SetRelativeRotation(FRotator(90, 0, 0));
	previewLine->SetRelativeLocation(FVector(-500, 0, 0));
	previewLine->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASkill::BeginPlay()
{
	Super::BeginPlay();

	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	myPlayerControler = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	mySkill = myPlayer->abilityComp->GetSkillInfo(skillType);
	if (skillType == SkillType::FireStorm || skillType == SkillType::Healing)
		skillDurationTime = 5.0f;

	//SkillEffect->bAutoDestroy = false;
	//SkillPreview->DecalSize.Y = enemySensor->GetShapeScale();
	Light->Intensity = 500.f;
	SkillEffect->SetActive(false, true);
	SkillEffect->Deactivate();
	SetActorHiddenInGame(true);
}

// Called every frame
void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (state == SkillState::Aiming) {
		SetLocatoin();
	}
}

void ASkill::OnSkill(bool useSkill)
{
	if (state == SkillState::Operating) return;

	if (useSkill) {
		state = SkillState::Aiming;
		SetActorHiddenInGame(false);
		OnEffect(false);
	}
	else {
		state = SkillState::Standby;
		SetActorHiddenInGame(true);
	}
}

void ASkill::TriggerSkill()
{
	if ( beginLoopTime == 0.f) {
		beginLoopTime = GetWorld()->GetTimeSeconds();
		state = SkillState::Operating;

		OnEffect(true);
	}
	else if (GetWorld()->GetTimeSeconds() - beginLoopTime > skillDurationTime) {
		//TODO Á¾·á
		GetWorldTimerManager().ClearTimer(SkillLoopTimer);
		SetActorHiddenInGame(true);
		OnEffect(false);
		beginLoopTime = 0.f;
		state = SkillState::Standby;
		return;
	}

	int damage = mySkill->powerValue;
	if (skillDurationTime > 0.5f) damage /= 2;

	enemySensor->GetOverlappingActors(enemySensorArray);
	if (enemySensorArray.IsEmpty() == false) {
		for (auto i : enemySensorArray) {

			if (skillType == SkillType::Healing) {
				if (i->ActorHasTag(TEXT("Player"))) {
					ATPSPlayer* player = Cast<ATPSPlayer>(i);
					player->AddHP(damage);
				}
			}

			else if (i->ActorHasTag(TEXT("Enemy"))) {
				AEnemy* enemy = Cast<AEnemy>(i);
				enemy->OnDamage(damage, "", myPlayer);
			}


		}
	}

	GetWorldTimerManager().ClearTimer(SkillLoopTimer);
	GetWorldTimerManager().SetTimer(SkillLoopTimer, this, &ASkill::TriggerSkill, 0.5f, false);
}

void ASkill::SetLocatoin()
{
	hitResult = LineTrace();
	SetActorLocation(hitResult.ImpactPoint);
	//SkillPreview->SetWorldRotation(hitResult.ImpactNormal.Rotation());
}

void ASkill::OnEffect(bool isUsed)
{
	if (isUsed) {
		SkillPreview->SetHiddenInGame(true);
		Light->SetHiddenInGame(true);
		previewLine->SetHiddenInGame(true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), skillSound, GetActorLocation());
		SkillEffect->Activate(true);
		//SkillEffect->Replay
			
	}
	else {
		SkillPreview->SetHiddenInGame(false);
		Light->SetHiddenInGame(false);
		previewLine->SetHiddenInGame(false);

		SkillEffect->Deactivate();
	}
}

FHitResult ASkill::LineTrace()
{
	FVector startLocation = myPlayerControler->PlayerCameraManager->GetCameraLocation();
	FRotator roc = myPlayerControler->PlayerCameraManager->GetCameraRotation();
	
	FVector endLocation = startLocation + (roc.Vector() * 2000);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, endLocation, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true);

	return HitResult;
}

