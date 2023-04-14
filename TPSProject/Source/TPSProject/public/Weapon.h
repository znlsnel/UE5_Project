// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WeaponData.h"
#include <Chaos/ChaosEngineInterface.h>

#include "NiagaraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


UCLASS()
class TPSPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Initialization(class ATPSPlayer* player);
	
public:
	class ATPSPlayer* myPlayer;

	UPROPERTY(EditAnywhere, category = weaponMesh)
		class USkeletalMeshComponent* weaponMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		 WeaponType weaponType = WeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		AmmoType ammoType = AmmoType::PistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		WeaponSlotType weaponSlotType = WeaponSlotType::SecondarySlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponTypes)
		UTexture* weaponIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponTypes)
		UTexture* ammoIcon;

	// 총을 쏜 후 다시 총을 쏠수 있을 때까지의 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		float fireDelay = 10.f;

	// 한번에 발사되는 총알의 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		int FireBulletCount = 0;

	// 매 사격시 정확도가 낮아지는 정도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		float FireSpread = 0.f;

	// 총을 장착할 캐릭터의 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		FName attachCharacterSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		TSubclassOf<UCameraShakeBase> FireCamShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UNiagaraSystem* TracerNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		TMap<TEnumAsByte<EPhysicalSurface>, FImpactInfo> arrayPhysicalSurface;
		//TEnumAsByte<EPhysicalSurface> ImpactInfoMap = EPhysicalSurface::SurfaceType1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		int Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		int MagazineSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UAnimMontage* CharacterFireAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UAnimMontage* CharacterADSFireAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UAnimMontage* CharacterReloadAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UAnimMontage* CharacterEquipAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UAnimationAsset* WeaponFireAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UAnimationAsset* WeaponReloadAnim;

};
