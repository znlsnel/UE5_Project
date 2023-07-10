// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemStoreUI.generated.h"

/**
 * 
 */
UENUM()
enum class EListType : uint8 {
	All,
	Weapon,
	Ammo,
	Items,
};

UCLASS()
class TPSPROJECT_API UItemStoreUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct()override;
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

	void UpdateButtonImage();
	void SetButtonImage(class UButton* button);

public:
	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* ItemInListPopup;

	TArray<class UButton*> buttons;
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

	EListType currListType = EListType::All;
};
