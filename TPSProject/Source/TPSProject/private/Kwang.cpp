// Fill out your copyright notice in the Description page of Project Settings.


#include "Kwang.h"
#include "KwangAnim.h"
#include "AttackRangeIndicator.h"

#include <particles/ParticleSystemComponent.h>

AKwang::AKwang() : Super()
{
	Tags.Add("Enemy");
}

void AKwang::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 15; i++) {
		AActor* tempActor = GetWorld()->SpawnActor(attackRangeIndicatorFactory);
		if (IsValid(tempActor)) {
			tempActor->SetActorHiddenInGame(true);
			attackRangeIndicators.Add(Cast<AAttackRangeIndicator>(tempActor));

		}
	}

	myAnim = Cast<UKwangAnim>(GetMesh()->GetAnimInstance());
}

AAttackRangeIndicator* AKwang::GetAttackRangeIndicator()
{
	for (auto actor : attackRangeIndicators)
		if (actor->isActiveDecal == false) {
			actor->isActiveDecal = true;
			return actor;
		}

	return nullptr;
}

void AKwang::LoopLightningStrike()
{
	GetWorldTimerManager().ClearTimer(lightningLoopTimer);
	if (fsm->target == nullptr || isLightningLoop == false || myAnim->isDead) {

		return;
	}

	AAttackRangeIndicator* attackRange =  GetAttackRangeIndicator();

	if (attackRange == nullptr)
		return;

	attackRange->SetActive(true, fsm->target->GetActorLocation());
	attackRange->SetMode(false);
	attackRange->SetAttackTimer(this);

	GetWorldTimerManager().SetTimer(lightningLoopTimer, this, & AKwang::LoopLightningStrike, 1.f, false);
}


