// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStoreUI.h"
#include "WeaponData.h"
#include "StatueAbilityWidget.h"
#include "AbilityUpgradeWidget.h"
#include "PlayerAbilityComp.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */

USTRUCT()
struct TPSPROJECT_API FFieldItem
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		FVector Location;
	UPROPERTY(VisibleAnywhere)
		FRotator rotation;
	UPROPERTY(VisibleAnywhere)
		EItemID ItemID = EItemID::Shotgun;
};

USTRUCT(BlueprintType)
struct TPSPROJECT_API FWeaponAmmo
{
	GENERATED_BODY()

public:
	UPROPERTY()
		WeaponType weaponType = WeaponType::None;

	UPROPERTY()
		int currAmmo = -1;
	UPROPERTY()
		int TotalAmmo = 0;
};

UCLASS()
class TPSPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		FString SaveFileName = "";

	UPROPERTY(VisibleAnywhere)
		FVector Location = FVector(0);

	UPROPERTY(VisibleAnywhere)
		int playerGrace = 0;

	UPROPERTY(VisibleAnywhere)
		int playerMineral = 0;

	UPROPERTY(VisibleAnywhere)
		int playerHp = 0;

	UPROPERTY(VisibleAnywhere)
		int currRound = 0;


	UPROPERTY()
		TArray<FWeaponAmmo> weaponAmmoInfo;


	UPROPERTY(VisibleAnywhere)
		FUpgradeInfo upgradeInfo;
	UPROPERTY(VisibleAnywhere)
		int ownWeapon = 0;

	UPROPERTY(VisibleAnywhere)
		TArray<FSkillInfo> skillInfos;

	UPROPERTY(VisibleAnywhere)
		TArray<FStatueAbility> statueAbilitys;
	UPROPERTY(VisibleAnywhere)
		float abilityRepairRate = 0.f;
	UPROPERTY(VisibleAnywhere)
		int statueHp = 0;


	UPROPERTY(VisibleAnywhere)
		TArray<int> InventoryItemID;

	UPROPERTY(VisibleAnywhere)
		TArray<FFieldItem> FieldItemID;



};
