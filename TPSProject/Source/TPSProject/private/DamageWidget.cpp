// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidget.h"

#include <Kismet/GameplayStatics.h>

UDamageWidget::UDamageWidget(const FObjectInitializer& ObjectInitialize) : Super(ObjectInitialize)
{
	
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void UDamageWidget::InitDamageWidget(FVector EnemyWorldLocation, int damage)
{
	EnemyPos = EnemyWorldLocation;
	this->Damage = damage;
	isInView = true;
	SetLocationLoop();
	OpenWidget();

	GetWorld()->GetTimerManager().SetTimer(initTimer, FTimerDelegate::CreateLambda([&]() {
		isInView = false;
		}), 1.f, false);
}

void UDamageWidget::SetLocationLoop()
{
	if (isInView == false) {
		GetWorld()->GetTimerManager().ClearTimer(loopSetPositionTimer);
	}
	FVector2D EnemyScreenPosition;

	UGameplayStatics::ProjectWorldToScreen(playerController,
		EnemyPos,
		EnemyScreenPosition);

	//EnemyScreenPosition.X += (viewX / 2);
	//EnemyScreenPosition.Y += (viewY / 2);

	SetPositionInViewport(EnemyScreenPosition);

	GetWorld()->GetTimerManager().SetTimer(loopSetPositionTimer, this, &UDamageWidget::SetLocationLoop, 0.01f, true);
}
