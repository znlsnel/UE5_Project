// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildableItemCheckUI.generated.h"

/**
 * 
 */
UENUM()
enum class ButtonType : uint8
{
	InstallButton,
	RepositionButton,
	CancelButton,
};

UCLASS()
class TPSPROJECT_API UBuildableItemCheckUI : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct()override;
	virtual bool Initialize()override;

	void OpenCheckUI();
	
	void ClickButton(ButtonType type);

	UFUNCTION()
		void InstallButtonClick() { ClickButton(ButtonType::InstallButton); };
	UFUNCTION()
		void RepositionButtonClick() { ClickButton(ButtonType::RepositionButton); };
	UFUNCTION()
		void CancelButtonClick() { ClickButton(ButtonType::CancelButton); };

public:
	FTimerHandle removeTimer;
	FTimerHandle InitTimer;


	UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* OpenAnim;

	UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* CloseAnim;


	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UButton* InstallButton;

	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UButton* RepositionButton;

	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UButton* CancelButton;

	class ABuildableItem* parentItem;
};
