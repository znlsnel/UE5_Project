// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStoreUI.h"
#include "StoreBuyItemWidget.h"

#include <Components/Button.h>
#include <Components/ListView.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <StoreListItemObject.h>
#include <Engine/Texture2D.h>
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UItemStoreUI::UItemStoreUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LoadJsonFile();

}

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
	currListType = EItemType::None;
	SetButtonImage(ListButton_All);

}

bool UItemStoreUI::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	buyItemWidget = CreateWidget< UStoreBuyItemWidget>(GetWorld(), buyItemWidgetFactory);
	UpdateItemObject();
	return true;
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
	currListType = EItemType::None;
	SetButtonImage(ListButton_All);

}

void UItemStoreUI::ClickListButton_Weapon()
{
	currListType = EItemType::weapon;
	SetButtonImage(ListButton_Weapon);

}

void UItemStoreUI::ClickListButton_Ammo()
{
	currListType = EItemType::ammo;
	SetButtonImage(ListButton_Ammo);

}

void UItemStoreUI::ClickListButton_Items()
{
	currListType = EItemType::items;
	SetButtonImage(ListButton_Items);

}


void UItemStoreUI::OffBuyItemWidget()
{
	if (buyItemWidget && buyItemWidget->IsInViewport())
		buyItemWidget->RemoveFromParent();
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

	UpdateItemList();
}

void UItemStoreUI::LoadJsonFile()
{
	FString JsonString; 
	FString filepath = FPaths::ProjectContentDir() + "\\Assets\\Json\\storeItemList.json";
	FFileHelper::LoadFileToString(JsonString, *filepath); 

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TArray<TSharedPtr<FJsonValue>> JsonParsed; 

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		for (auto& RowData : JsonParsed) {
			TSharedPtr<FJsonObject> row = RowData->AsObject();
			FStoreItem item;
			item.ItemName = row->GetStringField("ItemName");

			const TCHAR* texturePath = *(row->GetStringField("ItemIcon"));
			const FString path(row->GetStringField("ItemIcon"));

			ConstructorHelpers::FObjectFinder<UTexture2D> tempTexture(*path);

			if (tempTexture.Succeeded())
				item.ItemIcon = tempTexture.Object;

			item.GraceCost = row->GetIntegerField("GraceCost");
			item.MineralCost = row->GetIntegerField("MineralCost");
			item.ItemID = ConvertStringToItemID(row->GetStringField("ItemID"));
			item.itemType = ConvertStringToItemType(row->GetStringField("itemType"));
			ItemList.Add(item);
		}
	}
}

void UItemStoreUI::UpdateItemObject()
{
	for (auto ItemInfo : ItemList) {
		UStoreListItemObject* temp = NewObject<UStoreListItemObject>(this);
		temp->itemIfno = ItemInfo;
		temp->parentStore = this;
		ItemObjects.Add(temp);
	}
}

EItemID UItemStoreUI::ConvertStringToItemID(FString id)
{
	if (id == "Pistol")
		return EItemID::Pistol;

	if (id == "Shotgun")
		return EItemID::Shotgun;

	if (id == "Rifle")
		return EItemID::Rifle;

	if (id == "Bow")
		return EItemID::Bow;



	if (id == "PistolAmmo")
		return EItemID::PistolAmmo;

	if (id == "ShotgunAmmo")
		return EItemID::ShotgunAmmo;

	if (id == "RifleAmmo")
		return EItemID::RifleAmmo;

	if (id == "Arrow")
		return EItemID::Arrow;



	if (id == "StoneWall")
		return EItemID::StoneWall;

	if (id == "Barricade")
		return EItemID::Barricade;

	if (id == "ConcreteWall")
		return EItemID::ConcreteWall;

	if (id == "SendBagWall")
		return EItemID::SendBagWall;

	if (id == "SendBagWall")
		return EItemID::SendBagWall;

	return EItemID::Turret;
}

EItemType UItemStoreUI::ConvertStringToItemType(FString type)
{
	if (type == "weapon")
		return EItemType::weapon;
	
	else if (type == "ammo")
		return EItemType::ammo;
	
	else	if (type == "items")
		return EItemType::items;

	return EItemType::None;
}

void UItemStoreUI::UpdateItemList( )
{
	ItemListView->ClearListItems();
	for (auto Item : ItemObjects) {
		EItemType currType = Item->itemIfno.itemType;
		if (currListType == EItemType::None || currType == currListType) {
			ItemListView->AddItem(Item);
		}
	}
}
