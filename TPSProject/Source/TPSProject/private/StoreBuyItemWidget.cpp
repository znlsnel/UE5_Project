// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreBuyItemWidget.h"
#include "StoreListItem.h"
#include "TPSPlayer.h"

#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void UStoreBuyItemWidget::NativeConstruct()
{
	createItemButton->OnClicked.AddDynamic(this, &UStoreBuyItemWidget::ClickCreateItemButton);
	cancelButton->OnClicked.AddDynamic(this, &UStoreBuyItemWidget::ClickCancelButton);
	resultButton->OnClicked.AddDynamic(this, &UStoreBuyItemWidget::ClickResultButton);
}

void UStoreBuyItemWidget::OpenWidget(UStoreListItem* item)
{
	parentItem = item;
	FVector2D wigetPos = FVector2D(0);

	APlayerController* temp = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	temp->GetMousePosition(wigetPos.X, wigetPos.Y);

	int32 viewX = 0;
	int32 viewY = 0;
	temp->GetViewportSize(viewX, viewY);

	wigetPos.X = viewX / 2;

	SetPositionInViewport(wigetPos);
	if (IsInViewport() == false)
		AddToViewport();
	PlayAnimation(OpenAnim);
}

void UStoreBuyItemWidget::ClickCreateItemButton()
{
	player = Cast< ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FStoreItem temp = parentItem->ItemInfo;

	bool result = false;
	player->BuyItem(temp.ItemID, temp.GraceCost, temp.MineralCost, parentItem->ItemCount, result);

	OpenResultButton(result);
	//GetWorld()->GetTimerManager().ClearTimer(buyItemTimer);
	//GetWorld()->GetTimerManager().SetTimer(buyItemTimer, FTimerDelegate::CreateLambda(
	//	[&]() {
	//	}
	//), 0.2f, false);
}

void UStoreBuyItemWidget::ClickCancelButton()
{
	RemoveFromParent();
}

void UStoreBuyItemWidget::ClickResultButton()
{
	RemoveFromParent();
}

void UStoreBuyItemWidget::OpenResultButton(bool success)
{
	FLinearColor color;
	if (success) {
		color = FLinearColor(0.376471, 1.0, 0.447059, 1.f);
		resultString = FString("Success !");
	}
	else {
		color = FLinearColor(1.0, 0.353479, 0.3125, 1.f);
		resultString = FString("failed !");
	}
	resultButton->SetBackgroundColor(color);
	PlayAnimation(SuccessOpenAnim);
}
