// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenu.generated.h"

/**
 * 
 */

UENUM()
enum class ESelectSlot : uint8 {
	FirstSlot = 1,
	SecondSlot = 2,
	ThirdSlot = 3,
	None,
};

UCLASS()
class TPSPROJECT_API UStartMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;
	virtual bool Initialize()override;
	void UpdateSlotName();

	UFUNCTION()
		void ClickStartGameButton();
	UFUNCTION()
		void ClickLoadButton();
	UFUNCTION()
		void ClickContinueGameButton();
	UFUNCTION()
		void ClickQuitGameButton();
	UFUNCTION()
		void ClickFirstSlotButton();
	UFUNCTION()
		void ClickSecondSlotButton();
	UFUNCTION()
		void ClickThirdSlotButton();

	UFUNCTION()
		void ClickStartMenuButton();

	void UpdateSelectArrow();
	UFUNCTION(BlueprintCallable)
		void OpenWidget(bool GameOver, class ULevelSequencePlayer* gameOverSequencePlayer = nullptr);
public:
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UPanelWidget* MainPanel;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UPanelWidget* GameOverPanel;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UPanelWidget* SaveSlotPanel;
// Buttons
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* StartGameButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* ContinueGameButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* QuitGameButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* FirstSlot;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* SecondSlot;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* ThirdSlot;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* LoadButton;
	UPROPERTY(BlueprintReadOnly, Category = "BindWidgets", meta = (BindWidget))
		class UButton* StartMenuButton;
// Image
	UPROPERTY(BlueprintReadOnly, Category = "BindWigets", meta = (BindWidget))
		class UImage* FirstSlotArrow;
	UPROPERTY(BlueprintReadOnly, Category = "BindWigets", meta = (BindWidget))
		class UImage* SecondSlotArrow;
	UPROPERTY(BlueprintReadOnly, Category = "BindWigets", meta = (BindWidget))
		class UImage* ThirdSlotArrow;
	UPROPERTY(BlueprintReadOnly, Category = "BindWigets", meta = (BindWidget))
		class UImage* BackGroundImage;

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

	ESelectSlot currSelectSlot = ESelectSlot::None;
	class ATPSProjectGameModeBase* myGameMode;
	class ULevelSequencePlayer* sequencePlayer;

	UPROPERTY(EditDefaultsOnly)
		class USoundBase* startMusic;
	UPROPERTY()
		class UAudioComponent* startMusicAudio;


};
