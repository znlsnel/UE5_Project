// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueHpWidget.h"
#include "Doomstone.h"

#include <Slate/SceneViewport.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include <Widgets/SViewport.h>

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

	FSceneViewport* sceneViewport = GEngine->GameViewport->GetGameViewport();
	TSharedPtr<SViewport> ViewportWidget = sceneViewport->GetViewportWidget().Pin();
	FGeometry ViewportGeomtry = ViewportWidget->GetCachedGeometry();
	FVector2D ViewportSize = ViewportGeomtry.GetDrawSize();

	//if (parentScreenPos.X <= 1 || ViewportSize.X < parentScreenPos.X || parentScreenPos.Y <= 1 || ViewportSize.Y < parentScreenPos.Y){
	//	parentScreenPos.X = ViewportSize.X / 2;
	//	parentScreenPos.Y = 50;

	//}

	if (parentScreenPos.X <= 130)
		parentScreenPos.X = 130;
	else if (parentScreenPos.X > ViewportSize.X - 130)
		parentScreenPos.X = ViewportSize.X - 130;

	if (parentScreenPos.Y <= 50)
		parentScreenPos.Y = 50;
	else if (parentScreenPos.Y > ViewportSize.Y - 50)
		parentScreenPos.Y = ViewportSize.Y - 50;


	SetPositionInViewport(parentScreenPos);

	Hp = parent->Hp;
	MaxHp = parent->MaxHp;
	HpPercent = (float)Hp / (float)MaxHp;

	GetWorld()->GetTimerManager().SetTimer(LoopTimer, this, &UStatueHpWidget::UpdateWidgetPos, 0.01f, false);
}

void UStatueHpWidget::InitHpWidget()
{
	AddToViewport();
	parent->MakeStatueActor();
	UpdateWidgetPos();
}
