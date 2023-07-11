// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "ItemStoreUI.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemID : uint8
{
	Pistol = 0,
	Shotgun = 1,
	Rifle = 2,
	Bow = 3,

	PistolAmmo = 10,
	ShotgunAmmo = 11,
	RifleAmmo = 12,
	Arrow = 13,

	StoneWall = 20,
	Barricade = 21,
	ConcreteWall = 22,
	SendBagWall = 23,

	Turret = 30,
};

UENUM(BlueprintType)
enum class EItemType : uint8 {
	weapon,
	ammo,
	items,
	None,
};


USTRUCT(BlueprintType)
struct TPSPROJECT_API FStoreItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int GraceCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MineralCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemID ItemID = EItemID::Arrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType itemType = EItemType::ammo;
};


UCLASS()
class TPSPROJECT_API UItemStoreUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemStoreUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct()override;
	virtual bool Initialize()override;
	UFUNCTION(BlueprintCallable)
		void GetMouseInput();

	UFUNCTION(BlueprintCallable)
		void ClosePopup();

	UFUNCTION()
		void ClickListButton_All();
	UFUNCTION()
		void ClickListButton_Weapon();
	UFUNCTION()
		void ClickListButton_Ammo();
	UFUNCTION()
		void ClickListButton_Items();
	UFUNCTION(BlueprintCallable)
		void OffBuyItemWidget();

	void SetButtonImage(class UButton* button);
	void LoadJsonFile();
	void UpdateItemObject();
	EItemID ConvertStringToItemID(FString id);
	EItemType ConvertStringToItemType(FString type);

	void UpdateItemList();

public:
	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* ItemInListPopup;

	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UButton* ListButton_All;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UButton* ListButton_Weapon;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UButton* ListButton_Ammo;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UButton* ListButton_Items;

	UPROPERTY(EditDefaultsOnly)
		UTexture2D* ButtonImage_NonClick;
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* ButtonImage_Click;

	EItemType currListType = EItemType::None;

	TArray<class UButton*> buttons;
	TArray<FStoreItem> ItemList;
	TArray < TObjectPtr<class UStoreListItemObject>> ItemObjects;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<class UListView> ItemListView;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UStoreBuyItemWidget> buyItemWidgetFactory;
	UPROPERTY()
		class UStoreBuyItemWidget* buyItemWidget;
};
