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

	UFUNCTION(BlueprintImplementableEvent)
		void UnLockSkill(SkillType type);
	
	UFUNCTION(BlueprintImplementableEvent)
		void ToggleSkillSlot(SkillType type, bool isUse);

	void Initialization(class ATPSPlayer* player);
	void ToggleInventory();
	void ToggleInventory(bool On);

	template<SkillType type>
	void UseSkillSlot() { UseSkillSlot(type); };
	void UseSkillSlot(SkillType type);
	
public:
	class ATPSPlayer* myPlayer;
	class UPlayerAbilityComp* myAbilityComp;
	FTimerHandle startTimerHandle;

	UPROPERTY(BlueprintReadOnly)
		FString IceAttackTimeText ="";

	UPROPERTY(BlueprintReadOnly)
		FString LightningStrikeTimeText = "";

	UPROPERTY(BlueprintReadOnly)
		FString HealTimeText = "";

	UPROPERTY(BlueprintReadOnly)
		FString FireStormTimeText = "";


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



	// ÇÔ¼ö ##############################################

	UFUNCTION(BlueprintNativeEvent)
		void WeaponSwap();
	void WeaponSwap_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_ToggleInventory();

	UFUNCTION(BlueprintCallable)
		FString UpdateAmmoCount();

};
