// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerUI : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	virtual void InitializeComponent()override;
	virtual void BeginPlay()override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent)override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;

	void InitPlayerUI();

	void GetMouseInput();
	void ToggleMouse();
	void ToggleMouse(bool ActiveMouse);

	template <bool b>
	void ATVWeaponSelectUI() { ATVWeaponSelectUI(b); }
	void ATVWeaponSelectUI(bool addToView);

	void ATVWidgets();

	void InitializeWidgets();
	void OnOffOptionWidget();

	void ToggleInventory();
	void ToggleInventory(bool On);
	
	void GameOver();
	void GameOverSequencePauseEvent();
public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UScreenUI> ScreenUIFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UScreenUI* screenUI;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCrosshair> crosshairFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UCrosshair* crosshair;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UWeaponUI> weaponUIFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UWeaponUI* weaponSelectUI;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UStatueHpWidget> StatueHpFactory;
	UPROPERTY()
		class UStatueHpWidget* StatueHpWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UStatueAbilityWidget> statueAbilityWidgetFactory;
	UPROPERTY()
		class UStatueAbilityWidget* statueAbilityWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UExcOptionWidget> excOptionWidgetFactory;
	UPROPERTY()
		class UExcOptionWidget* excOptionWidget;
	bool IsMouseActive = false;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UStartMenu> startMenuFactory;
	UPROPERTY()
		class UStartMenu* startMenu;

	UPROPERTY(EditDefaultsOnly)
		class ULevelSequence* gameOverSequence;
	class ALevelSequenceActor* MySequenceActor;
	class ULevelSequencePlayer* sequencePlayer;
	FTimerHandle GameOverTimer;
};
