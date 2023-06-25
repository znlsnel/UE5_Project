// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHpBar.h"
#include "EnemyFSM.h"
#include "Enemy.h"

#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>

UEnemyHpBar::UEnemyHpBar(const FObjectInitializer& ObjectInitialize) : Super(ObjectInitialize)
{
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	isInView = false;
}

void UEnemyHpBar::InitHpWidget(AEnemy* enemy, float currHpPct, float preHpPct)
{
	myEnemy = enemy;
	if (myEnemy == nullptr) return;
	currHpPercent = currHpPct;
	preHpPercent = preHpPct;

	isInView = true;
	OpenWidget();
	SetLocationLoop();

	GetWorld()->GetTimerManager().ClearTimer(InitTimer);
	GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([&]() {
		isInView = false;
		CloseWidget();
		}), 2.f, false);
}

void UEnemyHpBar::SetLocationLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(LoopTimer);
	if (isInView == false) {
		return;
	}

	FVector2D EnemyScreenPosition = FVector2D(0);

	FVector enemyPos = myEnemy->GetActorLocation();
	enemyPos.Z += (myEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2) + 30;

	UGameplayStatics::ProjectWorldToScreen(playerController,
		enemyPos,
		EnemyScreenPosition);

	preHpPercent = FMath::FInterpTo(preHpPercent, currHpPercent, GetWorld()->GetDeltaSeconds(), uiRenderTime - 0.5f);

	SetPositionInViewport(EnemyScreenPosition);


	GetWorld()->GetTimerManager().SetTimer(LoopTimer, this, &UEnemyHpBar::SetLocationLoop, 0.01f, false);
}
