// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreListItem.h"
#include "StoreListItemObject.h"
#include "StoreBuyItemWidget.h"

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>

void UStoreListItem::NativeConstruct()
{
	mainButton->OnClicked.AddDynamic(this, &UStoreListItem::ClickItemMainButton);

	ItemCountUpButton->OnClicked.AddDynamic(this, &UStoreListItem::ClickItemCountUpButton);
	ItemCountUpButton->OnHovered.AddDynamic(this, &UStoreListItem::HorveredItemCountUpButton);
	ItemCountUpButton->OnUnhovered.AddDynamic(this, &UStoreListItem::UnHorveredItemCountUpButton);

	ItemCountDownButton->OnClicked.AddDynamic(this, &UStoreListItem::ClickItemCountDownButton);
	ItemCountDownButton->OnHovered.AddDynamic(this, &UStoreListItem::HorveredItemCountDownButton);
	ItemCountDownButton->OnUnhovered.AddDynamic(this, &UStoreListItem::UnHorveredItemCountDownButton);
}

void UStoreListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UStoreListItemObject* temp = Cast<UStoreListItemObject>(ListItemObject);
	if (IsValid(temp) == false)
		return;
	
	ItemInfo = temp->itemIfno;
	PlayAnimation(activeAnim);
	iconImage->SetBrushFromTexture(ItemInfo.ItemIcon);
	itemName->SetText(FText::FromString(ItemInfo.ItemName));
	ItemCount = 1;
	GraceCost = ItemInfo.GraceCost;
	MineralCost = ItemInfo.MineralCost;
	parentStore = temp->parentStore;
	isHideUpButton = true;
	isHideDownButton = true;
	parentStore->OffBuyItemWidget();
	isWeaponItem = ItemInfo.itemType == EItemType::weapon;
	if (isWeaponItem) {
		ItemCountBox->SetRenderScale(FVector2D(0, 0));
	}
	else {
		ItemCountBox->SetRenderScale(FVector2D(1, 1));

	}

}

void UStoreListItem::ClickItemMainButton()
{
	parentStore->buyItemWidget->OpenWidget(this);
}

void UStoreListItem::ClickItemCountUpButton()
{
	ItemCount++;
	GraceCost = ItemInfo.GraceCost * ItemCount;
	MineralCost = ItemInfo.MineralCost * ItemCount;
}

void UStoreListItem::HorveredItemCountUpButton()
{
	if (isHideUpButton) {
		PlayAnimation(ActiveUpButton);
		isHideUpButton = false;
	}
}

void UStoreListItem::UnHorveredItemCountUpButton()
{
	if (isHideUpButton == false) {
		PlayAnimation(HideUpButton);
		isHideUpButton = true;
	}
}

void UStoreListItem::ClickItemCountDownButton()
{
	if (ItemCount > 1)
		ItemCount--;
	GraceCost = ItemInfo.GraceCost * ItemCount;
	MineralCost = ItemInfo.MineralCost * ItemCount;
}

void UStoreListItem::HorveredItemCountDownButton()
{
	if (isHideDownButton) {
		PlayAnimation(ActiveDownButton);
		isHideDownButton = false;
	}
}

void UStoreListItem::UnHorveredItemCountDownButton()
{
	if (isHideDownButton == false) {
		PlayAnimation(HideDownButton);
		isHideDownButton = true;
	}
}
