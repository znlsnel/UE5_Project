// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStoreUI.h"
#include "Blueprint/UserWidget.h"
#include <Blueprint/IUserObjectListEntry.h>
#include "StoreListItem.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UStoreListItem : public UUserWidget , public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
		void ClickItemMainButton();
	UFUNCTION()
		void ClickItemCountUpButton();
	UFUNCTION()
		void HorveredItemCountUpButton();
	UFUNCTION()
		void UnHorveredItemCountUpButton();

	UFUNCTION()
		void ClickItemCountDownButton();
	UFUNCTION()
		void HorveredItemCountDownButton();
	UFUNCTION()
		void UnHorveredItemCountDownButton();

public:
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* activeAnim;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* ActiveUpButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* HideUpButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* ActiveDownButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* HideDownButton;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		TObjectPtr<class UImage> iconImage;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		TObjectPtr<class UTextBlock> itemName;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		TObjectPtr<class UVerticalBox> ItemCountBox;



// Button
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		TObjectPtr<class UButton> mainButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		TObjectPtr<class UButton> ItemCountUpButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		TObjectPtr<class UButton> ItemCountDownButton;

	UPROPERTY(BlueprintReadOnly)
		int ItemCount = 1;
	UPROPERTY(BlueprintReadOnly)
		int GraceCost = 1;
	UPROPERTY(BlueprintReadOnly)
		int MineralCost = 1;


	UPROPERTY(BlueprintReadOnly)
		FStoreItem ItemInfo;

	bool isHideUpButton = true;
	bool isHideDownButton = true;

	class UItemStoreUI* parentStore;
	bool isWeaponItem = false;
};
