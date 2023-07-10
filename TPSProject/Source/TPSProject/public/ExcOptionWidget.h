// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StartMenu.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExcOptionWidget.generated.h"

/**
 * 
 */


UCLASS()
class TPSPROJECT_API UExcOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;
	virtual bool Initialize()override;
	void OpenWidget();
	void CloseWidget();

	UFUNCTION()
		void ClickFirstSlotButton();
	UFUNCTION()
		void ClickSecondSlotButton();
	UFUNCTION()
		void ClickThirdSlotButton();
	
	UFUNCTION()
		void ClickSaveButton();
	UFUNCTION()
		void ClickLoadButton();

	UFUNCTION()
		void ClickSaveSlotOpenButton();
	UFUNCTION()
		void ClickCloseButton();
	UFUNCTION()
		void ClickCloseInputTextSlotButton();
	UFUNCTION()
		void ClickInputTextSlotCheckButton();

	void UpdateSelectArrow(bool Initialize = false);
	void UpdateSlotName();
public:
	FTimerHandle closeTimer;
	class ATPSProjectGameModeBase* myGameMode;
	class ATPSPlayer* player;

// WidgetAnimations
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OpenOption;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* CloseOption;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OpenSaveSlotPanel;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* CloseSaveSlotPanel;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OpenInputTextSlot;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* CloseInputTextSlot;


// Buttons
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* FirstSlot;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* SecondSlot;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* ThirdSlot;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* SaveButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* LoadButton;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* SaveSlotOpenButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* CloseButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* CloseInputTextSlotButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* InputTextSlotCheckButton;

// Images
	UPROPERTY(BlueprintReadOnly, Category = "BindWigets", meta = (BindWidget))
		class UImage* FirstSlotArrow;
	UPROPERTY(BlueprintReadOnly, Category = "BindWigets", meta = (BindWidget))
		class UImage* SecondSlotArrow;
	UPROPERTY(BlueprintReadOnly, Category = "BindWigets", meta = (BindWidget))
		class UImage* ThirdSlotArrow;

// FStrings
	UPROPERTY(BlueprintReadOnly)
		FString FirstSaveSlotName = "";
	UPROPERTY(BlueprintReadOnly)
		FString FirstSaveSlotDay = "";
	UPROPERTY(BlueprintReadOnly)
		FString SecondSaveSlotName = "";
	UPROPERTY(BlueprintReadOnly)
		FString SecondSaveSlotDay = "";
	UPROPERTY(BlueprintReadOnly)
		FString ThirdSaveSlotName = "";
	UPROPERTY(BlueprintReadOnly)
		FString ThirdSaveSlotDay = "";
	UPROPERTY(BlueprintReadWrite)
		FString InputText = "";

	ESelectSlot currSelectSlot = ESelectSlot::None;
	bool isOpenSaveSlotPanel = false; 
};
