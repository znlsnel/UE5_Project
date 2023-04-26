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
	bool isInventoryOpen();
	void GetMouseInput();

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UScreenUI> ScreenUIFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UScreenUI* screenUI;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCrosshair> crosshairFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UCrosshair* crosshair;

	void ToggleInventory();
};
