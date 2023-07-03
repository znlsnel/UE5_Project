// Fill out your copyright notice in the Description page of Project Settings.


#include "Riktor.h"
#include "EnemyFSM.h"
#include "RiktorThunder.h"
#include "AttackRangeIndicator.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
ARiktor::ARiktor() : Super()
{
	targetPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ARiktor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 10; i++) {
		ARiktorThunder* tempActor = GetWorld()->SpawnActor<ARiktorThunder>(ThunderFactory);

		if (IsValid(tempActor)) {
			Thunders.Add(tempActor);
		}
		else
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No ARiktorThunder"));

	}

	rangeIndicator = Cast<AAttackRangeIndicator>(GetWorld()->SpawnActor(rangeIndicatorFactory));
	if (IsValid(rangeIndicatorFactory)) {
		rangeIndicator->SetActive(false);
		rangeIndicator->SetMode(true);
	}
	else
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No RangeIndicator"));

}

ARiktorThunder* ARiktor::GetThunder()
{
	for (auto thunder : Thunders) {
		if (thunder->isActiveThunder == false)
			return thunder;
	}
	return nullptr;
}

void ARiktor::SpawnThunderMonster()
{
	isSpawnThunder = true;
	SpawnThunderLoop();

	GetWorldTimerManager().ClearTimer(SpawnThunderEndTimer);
	GetWorldTimerManager().SetTimer(SpawnThunderEndTimer, FTimerDelegate::CreateLambda(
		[&]() {
			isSpawnThunder = false;
		}), 2.f, false);
}

void ARiktor::SpawnThunderLoop()
{
	GetWorldTimerManager().ClearTimer(SpawnThunderTimer);
	if (isSpawnThunder == false) {
		return;
	}

	FVector GenPos = GetMesh()->GetSocketLocation(FName("hand_l"));

	if (isLeftHand) {
		isLeftHand = false;
	}
	else {
		GenPos = GetMesh()->GetSocketLocation(FName("hand_r"));
		isLeftHand = true;
	}

	AActor* target = targetPlayer;
	if (target == nullptr)
		target = fsm->target;
	//TODO
	ARiktorThunder* tempThunder = GetThunder();
	if (tempThunder)
		tempThunder->SetActiveThunder(true, GenPos, target);
	else {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No ARiktorThunder"));
		return;
	}

	GetWorldTimerManager().SetTimer(SpawnThunderTimer, this, &ARiktor::SpawnThunderLoop, 0.1f, false);
}

void ARiktor::OnAttackRangeIndicator(bool trigger)
{
	if (trigger == false) {
		rangeIndicator->SetActive(true, GetActorLocation(), 5.f);
	}
	else {
		rangeIndicator->SetAttackTimer(this, 0.1f);
	}
}
