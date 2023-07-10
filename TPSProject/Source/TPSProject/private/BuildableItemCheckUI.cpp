// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildableItemCheckUI.h"
#include "BuildableItem.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Components/Button.h>

void UBuildableItemCheckUI::NativeConstruct()
{
	Super::NativeConstruct();

}

bool UBuildableItemCheckUI::Initialize()
{
	Super::Initialize();

	InstallButton->OnClicked.AddDynamic(this, &UBuildableItemCheckUI::InstallButtonClick);
	RepositionButton->OnClicked.AddDynamic(this, &UBuildableItemCheckUI::RepositionButtonClick);
	CancelButton->OnClicked.AddDynamic(this, &UBuildableItemCheckUI::CancelButtonClick);

	return false;
}

void UBuildableItemCheckUI::OpenCheckUI()
{
	if (IsInViewport() && IsValid(OpenAnim)) {
		PlayAnimation(OpenAnim);
	}
}

void UBuildableItemCheckUI::ClickButton(ButtonType type)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Click! bt"));
	if (parentItem == nullptr) return;

	switch (type) {
	case ButtonType::InstallButton:
		parentItem->completeBuilding(true);
		break;
	case ButtonType::RepositionButton:
		parentItem->completeBuilding(false);
		break;
	case ButtonType::CancelButton:
		parentItem->CancelBuilding();
		break;
	}

	if (IsInViewport() && IsValid(OpenAnim)) {
		PlayAnimation(CloseAnim);
		GetWorld()->GetTimerManager().ClearTimer(removeTimer);
		GetWorld()->GetTimerManager().SetTimer(removeTimer, FTimerDelegate::CreateLambda([&]() {
			if (IsInViewport())
				RemoveFromParent();
			}), 1.f, false);
	}

}
