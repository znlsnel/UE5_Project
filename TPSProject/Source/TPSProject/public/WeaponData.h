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
enum class WeaponType : uint8
{
	Pistol		UMETA(DisplayName="Pistol"),
	Rifle			UMETA(DisplayName = "Rifle"),
	Shotgun		UMETA(DisplayName = "Shotgun"),
};

UENUM(BlueprintType)
enum class AmmoType : uint8
{
	PistolAmmo		UMETA(DisplayName = "PistolAmmo"),
	RifleAmmo		UMETA(DisplayName = "RifleAmmo"),
	ShotgunAmmo	UMETA(DisplayName = "ShotgunAmmo"),
};

UENUM(BlueprintType)
enum class WeaponSlotType : uint8
{
	PrimarySlot		UMETA(DisplayName = "PrimarySlot"),
	SecondarySlot	UMETA(DisplayName = "SecondarySlot"),
};

USTRUCT(BlueprintType)
struct TPSPROJECT_API FWeaponInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Textrue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ZOrder;
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

