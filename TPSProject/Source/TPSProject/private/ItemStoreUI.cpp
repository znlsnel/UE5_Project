// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStoreUI.h"

#include <Components/Button.h>
#include <Blueprint/WidgetLayoutLibrary.h>

void UItemStoreUI::NativeConstruct()
{
	Super::NativeConstruct();
	ListButton_All->OnClicked.AddDynamic(this, &UItemStoreUI::ClickListButton_All);
	ListButton_Weapon->OnClicked.AddDynamic(this, &UItemStoreUI::ClickListButton_Weapon);
	ListButton_Ammo->OnClicked.AddDynamic(this, &UItemStoreUI::ClickListButton_Ammo);
	ListButton_Items->OnClicked.AddDynamic(this, &UItemStoreUI::ClickListButton_Items);

	buttons.Add(ListButton_All);
	buttons.Add(ListButton_Weapon);
	buttons.Add(ListButton_Ammo);
	buttons.Add(ListButton_Items);
	currListType = EListType::All;
	UpdateButtonImage();
}

void UItemStoreUI::GetMouseInput()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());


	if (ItemInListPopup && ItemInListPopup->IsVisible())
	{
		FGeometry Geometry = ItemInListPopup->GetCachedGeometry();

		if (!Geometry.IsUnderLocation(MousePosition) && ItemInListPopup->IsInViewport())
		{
			ClosePopup();
		}
	}
}

void UItemStoreUI::ClosePopup()
{
	if (ItemInListPopup && ItemInListPopup->IsInViewport())
	{
		ItemInListPopup->RemoveFromParent();
	}
}

void UItemStoreUI::ClickListButton_All()
{
	currListType = EListType::All;
	UpdateButtonImage();
}

void UItemStoreUI::ClickListButton_Weapon()
{
	currListType = EListType::Weapon;
	UpdateButtonImage();
}

void UItemStoreUI::ClickListButton_Ammo()
{
	currListType = EListType::Ammo;
	UpdateButtonImage();
}

void UItemStoreUI::ClickListButton_Items()
{
	currListType = EListType::Items;
	UpdateButtonImage();
}

void UItemStoreUI::UpdateButtonImage()
{
	switch (currListType) {
	case EListType::All: 
		SetButtonImage(ListButton_All);
		break;
	case EListType::Weapon:
		SetButtonImage(ListButton_Weapon);
		break;
	case EListType::Ammo:
		SetButtonImage(ListButton_Ammo);
		break;
	case EListType::Items:
		SetButtonImage(ListButton_Items);
		break;
	}
}

void UItemStoreUI::SetButtonImage(UButton* button)
{
	button->WidgetStyle.Normal.SetResourceObject(ButtonImage_Click);
	button->WidgetStyle.Hovered.SetResourceObject(ButtonImage_Click);
	button->WidgetStyle.Pressed.SetResourceObject(ButtonImage_Click);

	for (auto tempButton : buttons) {
		if (tempButton == button) continue;

		tempButton->WidgetStyle.Normal.SetResourceObject(ButtonImage_NonClick);
		tempButton->WidgetStyle.Hovered.SetResourceObject(ButtonImage_NonClick);
		tempButton->WidgetStyle.Pressed.SetResourceObject(ButtonImage_NonClick);
	}

}
