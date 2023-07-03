// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueHpWidget.h"
#include "Doomstone.h"

#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>

UStatueHpWidget::UStatueHpWidget(const FObjectInitializer& ObjectInitialize): Super(ObjectInitialize)
{
	Super::Initialize();
	playerCtrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	parent = Cast<ADoomstone>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoomstone::StaticClass()));
}

void UStatueHpWidget::NativeConstruct()
{
	UpdateWidgetPos();
}

void UStatueHpWidget::UpdateWidgetPos()
{

	if (parent == nullptr || playerCtrl == nullptr)
		return;

	GetWorld()->GetTimerManager().ClearTimer(LoopTimer);

	if (parent->isDestory){
		RemoveFromParent();
		return;
	}

	FVector parentPos = parent->GetActorLocation();
	parentPos.Z += 400;
	FVector2D parentScreenPos;
	UGameplayStatics::ProjectWorldToScreen(playerCtrl,
		parentPos,
		parentScreenPos);

	SetPositionInViewport(parentScreenPos);

	Hp = parent->Hp;
	MaxHp = parent->MaxHp;
	HpPercent = (float)Hp / (float)MaxHp;

	GetWorld()->GetTimerManager().SetTimer(LoopTimer, this, &UStatueHpWidget::UpdateWidgetPos, 0.01f, false);
}
