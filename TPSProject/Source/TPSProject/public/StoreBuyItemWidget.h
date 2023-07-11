// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoreBuyItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UStoreBuyItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;
	void OpenWidget(class UStoreListItem* Item);
	UFUNCTION()
		void ClickCreateItemButton();
	UFUNCTION()
		void ClickCancelButton();
	UFUNCTION()
		void ClickResultButton();

	void OpenResultButton(bool success);

public:
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OpenAnim;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* SuccessOpenAnim;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* SuccessCloseAnim;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget), Transient)
		TObjectPtr<class UButton> createItemButton;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget), Transient)
		TObjectPtr<class UButton> cancelButton;

	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget), Transient)
		TObjectPtr<class UButton> resultButton;

	class UStoreListItem* parentItem;
	class ATPSPlayer* player;

	FTimerHandle buyItemTimer;

	UPROPERTY(BlueprintReadOnly)
	FString resultString = "";

};
