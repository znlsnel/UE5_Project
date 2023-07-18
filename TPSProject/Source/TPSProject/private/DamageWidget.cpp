// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidget.h"

#include <Kismet/GameplayStatics.h>

bool UDamageWidget::isLeftAnim = true;
UDamageWidget::UDamageWidget(const FObjectInitializer& ObjectInitialize) : Super(ObjectInitialize)
{
	
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

}

void UDamageWidget::InitDamageWidget(FVector EnemyWorldLocation, int damage, bool isRecycle)
{
	if (IsInViewport() == false)
		AddToViewport();

	EnemyPos = EnemyWorldLocation;
	if (isRecycle)
		this->Damage += damage;
	else
		this->Damage = damage;

	isInView = true;
	//hpPercent = currHp;
	//DisappearHp = preHp;


	SetLocationLoop();
	StopAllAnimations();

	if(UDamageWidget::isLeftAnim)
		PlayAnimation(play_Left);
	else
		PlayAnimation(play_Right);
	UDamageWidget::isLeftAnim = UDamageWidget::isLeftAnim ? false : true;

	GetWorld()->GetTimerManager().ClearTimer(initTimer);
	GetWorld()->GetTimerManager().SetTimer(initTimer, FTimerDelegate::CreateLambda([&]() {
		isInView = false;
		RemoveFromParent();
		}), 2.f, false);
}

void UDamageWidget::SetLocationLoop()
{
	if (isInView == false) {
		GetWorld()->GetTimerManager().ClearTimer(loopSetPositionTimer);
		return;
	}

	FVector2D EnemyScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(playerController,
		EnemyPos,
		EnemyScreenPosition);

	//EnemyScreenPosition.X += (viewX / 2);
	//EnemyScreenPosition.Y += (viewY / 2);

	//DisappearHp = FMath::FInterpTo(DisappearHp, hpPercent, GetWorld()->GetDeltaSeconds(), uiRenderTime - 0.5f);

	SetPositionInViewport(EnemyScreenPosition);

	GetWorld()->GetTimerManager().SetTimer(loopSetPositionTimer, this, &UDamageWidget::SetLocationLoop, 0.01f, true);
}
