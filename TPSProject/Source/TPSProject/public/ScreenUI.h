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

	void ATVWidget();

	UFUNCTION(BlueprintImplementableEvent)
		void StartWidget();

	void Initialization(class ATPSPlayer* player);
	void SetupInputBinding(class UInputComponent* PlayerInputComponent);
	void ToggleInventory();
	void ToggleInventory(bool On);

public:
	class ATPSPlayer* myPlayer;

	FTimerHandle startTimerHandle;

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = Inventory)
		class UInventory* inventory;

	UPROPERTY(BlueprintReadWrite)
		bool bOpenInventory = false;
	UPROPERTY(EditAnywhere, blueprintReadWrite, category = Player)
		float hpRatio;
	UPROPERTY(EditAnywhere, blueprintReadWrite, category = Player)
		FString hpInfo;

	UFUNCTION(BlueprintImplementableEvent)
		void InventoryOnOff(bool On);

	UPROPERTY(EditAnywhere)
		UTexture2D* PistolUI;
	UPROPERTY(EditAnywhere)
		UTexture2D* RifleUI;
	UPROPERTY(EditAnywhere)
		UTexture2D* ShotgunUI;
	UPROPERTY(EditAnywhere)
		UTexture2D* BowUI;
	UPROPERTY(EditAnywhere)
		UTexture2D* SwordUI;

	UPROPERTY(BlueprintReadWrite)
		UTexture2D* currWeaponTexture;



	// �Լ� ##############################################

	UFUNCTION(BlueprintNativeEvent)
		void WeaponSwap();
	void WeaponSwap_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_ToggleInventory();

	UFUNCTION(BlueprintCallable)
		FString UpdateAmmoCount();
};
