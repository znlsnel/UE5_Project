// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include<Chaos/ChaosEngineInterface.h>

#include "NiagaraComponent.h"
#include "CoreMinimal.h"
#include "WeaponData.generated.h"

/**
 *
 */
class TPSPROJECT_API WeaponData
{
public:
	WeaponData();
	~WeaponData();
};

UENUM(BlueprintType)
enum class DashType : uint8
{
	W		UMETA(DisplayName = "W"),
	A		UMETA(DisplayName = "A"),
	S		UMETA(DisplayName = "S"),
	D		UMETA(DisplayName = "D"),
	None	UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class ItemType : uint8
{
	Weapon		UMETA(DisplayName = "Weapon"),
	Ammo		UMETA(DisplayName = "Ammo"),
	Grace		UMETA(DisplayName = "Grace"),
	Mineral		UMETA(DisplayName = "Mineral"),
	Building		UMETA(DisplayName = "Building"),
	None		UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class BuildableItemType : uint8
{
	StoneWall		UMETA(DisplayName = "StoneWall"),
	SandBag			UMETA(DisplayName = "SandBag"),
	Concrete		UMETA(DisplayName = "Concrete"),
	BrokenWall		UMETA(DisplayName = "BrokenWall"),
	Turret		UMETA(DisplayName = "Turret"),
	None			UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class BowState : uint8
{
	AimMode	UMETA(DisplayName = "AimMode"),
	TravelMode	UMETA(DisplayName = "TravelMode"),
	None		UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	Pistol		UMETA(DisplayName = "Pistol"),
	Rifle			UMETA(DisplayName = "Rifle"),
	Shotgun		UMETA(DisplayName = "Shotgun"),
	Bow			UMETA(DisplayName = "Bow"),
	Sword		UMETA(DisplayName = "Sword"),
	None		UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class AmmoType : uint8
{
	PistolAmmo		UMETA(DisplayName = "PistolAmmo"),
	RifleAmmo		UMETA(DisplayName = "RifleAmmo"),
	ShotgunAmmo	UMETA(DisplayName = "ShotgunAmmo"),
	None			UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class WeaponSlotType : uint8
{
	PrimarySlot		UMETA(DisplayName = "PrimarySlot"),
	SecondarySlot	UMETA(DisplayName = "SecondarySlot"),
	None			UMETA(DisplayName = "None"),
};


USTRUCT(BlueprintType)
struct TPSPROJECT_API FItemInfoinInventory
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* itemTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AmmoType ammoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		WeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		WeaponSlotType weaponSlotType;
};


USTRUCT(BlueprintType)
struct TPSPROJECT_API FImpactInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EPhysicalSurface> physicalSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ImpactDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* ImpactSound;

};

USTRUCT(BlueprintType)
struct TPSPROJECT_API FSpareAmmoInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AmmoType ammoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int maxAmmo;
};

