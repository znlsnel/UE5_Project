// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStoreUI.h"
#include <Blueprint/WidgetLayoutLibrary.h>

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
