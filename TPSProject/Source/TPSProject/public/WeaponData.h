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
	Pistol,
	Rifle,
	Shotgun
};

UENUM(BlueprintType)
enum class AmmoType : uint8
{
	PistolAmmo,
	RifleAmmo,
	ShotgunAmmo
};

UENUM(BlueprintType)
enum class WeaponSlotType : uint8
{
	PrimarySlot, // 주 무기 슬롯
	SecondarySlot, // 보조 무기 슬롯 
};


USTRUCT(BlueprintType)
struct FImpactInfo
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
struct FSpareAmmoInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<AmmoType> ammoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int maxAmmo;
};