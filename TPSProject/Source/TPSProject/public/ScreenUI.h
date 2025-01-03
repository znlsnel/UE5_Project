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

	UFUNCTION(BlueprintImplementableEvent)
		void playerHpEvent(bool isHeal);
	

	// SkillUI
	void UpdateSkillLockImage(class UPlayerAbilityComp* playerAC);
	void SetSkillLockImage(bool IsPossibleSkill, class UImage* skillImage, class UImage* SkillLockImage);

	UFUNCTION()
		void UpdateSkillSlotPressed(SkillType type, bool isUse = false);

	void SetSkillSlotPressed(class UImage* skillImage, class UImage* skillLockImage, bool SkillCooling);
	void SkillCoolTimeTextInit();

	UFUNCTION()
		UImage* GetSkillImage(SkillType type);
	UFUNCTION()
		UImage* GetSkillLockImage(SkillType type);

	UFUNCTION(BlueprintImplementableEvent)
		void RespawnEvent(bool death);
	UFUNCTION(BlueprintImplementableEvent)
		void RespawnTimeUpdate();

	void RespawnTimeLoop();
	void Initialization(class ATPSPlayer* player);
	void ToggleInventory();
	void ToggleInventory(bool On);

	template<SkillType type>
	void UseSkillSlot() { UseSkillSlot(type); };
	void UseSkillSlot(SkillType type);
	
	// dash - true, Shield - false
	void BasicSkillCoolTime(bool dashOrShield, float CoolTime);
	void BasicSkillCoolTimeLoop();

public:

	class ATPSPlayer* myPlayer;
	class UPlayerAbilityComp* myAbilityComp;
	FTimerHandle startTimerHandle;
	FTimerHandle respawnTimer;
	FTimerHandle basicSkillLoopTimer;
	FTimerHandle basicSkillLoopEndTimer;

	UPROPERTY(BlueprintReadOnly)
		FString IceAttackTimeText ="";

	UPROPERTY(BlueprintReadOnly)
		FString LightningStrikeTimeText = "";

	UPROPERTY(BlueprintReadOnly)
		FString HealTimeText = "";

	UPROPERTY(BlueprintReadOnly)
		FString FireStormTimeText = "";

	UPROPERTY(BlueprintReadOnly)
		float DashTime = 1.f;
	float DashCoolTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
		float ShieldTime = 1.f;
	float ShieldCoolTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
		int RespawnTime = 0;

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

	UPROPERTY(BlueprintReadWrite)
		int currDay = 1;

	UPROPERTY(BlueprintReadOnly)
		class ATPSProjectGameModeBase* myGameMode;

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

	bool bBasicSkillLoopEnd = false;

	// �Լ� ##############################################

	UFUNCTION(BlueprintNativeEvent)
		void WeaponSwap();
	void WeaponSwap_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_ToggleInventory();

	UFUNCTION(BlueprintCallable)
		FString UpdateAmmoCount();

	class UImage* currSelectSkill;

	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* IceAttackImage;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* FireStormImage;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* LightningStrikeImage;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* HealImage;

	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* IceAttackLockImage;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* FireStormLockImage;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* LightningStrikeLockImage;
	UPROPERTY(BlueprintReadOnly, Category = "BindButton", meta = (BindWidget))
		class UImage* HealLockImage;
};
