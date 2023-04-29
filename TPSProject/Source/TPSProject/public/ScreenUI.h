// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenUI.generated.h"



/**
 * 
 */
UCLASS()
class TPSPROJECT_API UScreenUI : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateScreenUI();
	void StartGame();
	void Initialization(class ATPSPlayer* player);
	void SetupInputBinding(class UInputComponent* PlayerInputComponent);
	void ToggleInventory();
public:
	class ATPSPlayer* myPlayer;

	FTimerHandle startTimerHandle;

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = Inventory)
		class UInventory* inventory;

	UPROPERTY(BlueprintReadOnly)
		bool bOpenInventory = false;
	UPROPERTY(EditAnywhere, blueprintReadWrite, category = Player)
		float hpRatio;
	UPROPERTY(EditAnywhere, blueprintReadWrite, category = Player)
		FString hpInfo;
	
	UPROPERTY(EditAnywhere)
		UTexture2D* PistolUI;
	UPROPERTY(EditAnywhere)
		UTexture2D* RifleUI;
	UPROPERTY(EditAnywhere)
		UTexture2D* ShotgunUI;
	UPROPERTY(EditAnywhere)
		UTexture2D* TransParentUI;

	UPROPERTY(BlueprintReadWrite)
		FWeaponInfo primaryInfo;

	UPROPERTY(BlueprintReadWrite)
		FWeaponInfo secondaryInfo;
	

	// ÇÔ¼ö ##############################################

	UFUNCTION(BlueprintNativeEvent)
		void WeaponSwap();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_ToggleInventory();

	UFUNCTION(BlueprintCallable)
		FString UpdateAmmoCount();
};
