// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Chaos/ChaosEngineInterface.h>

#include "Item.h"
#include "playerFire.h"
#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "WeaponData.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


UCLASS()
class TPSPROJECT_API AWeapon : public AItem
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
	virtual void SynchronizeWhitPlayer(ATPSPlayer* player);
	void UnSynchronizeWhitPlayer();
	virtual void Attack();
	FHitResult LineTrace();
	void HideWeapon();
	void UncoverWeapon();
	void DiscardWeaponIfAlreadyExists();
	void CreatePickupCollision();
	void RemovePickupCollision();
	void Reload();

public:
	UPlayerAnim* anim;
	bool isSynchronized = false;

	UPROPERTY(VisibleAnywhere, category = weaponMesh)
		class USkeletalMeshComponent* weaponMeshComp;


	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* pickupCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		 WeaponType weaponType = WeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		AmmoType ammoType = AmmoType::PistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		WeaponSlotType weaponSlotType = WeaponSlotType::SecondarySlot;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponTypes)
		UTexture2D* ammoIcon;

	// ���� �� �� �ٽ� ���� ��� ���� �������� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		float fireDelay = 10.f;

	// �ѹ��� �߻�Ǵ� �Ѿ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		int FireBulletCount = 0;

	// �� ��ݽ� ��Ȯ���� �������� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		float FireSpread = 0.f;

	// ���� ������ ĳ������ ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		FName attachCharacterSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		TSubclassOf<UCameraShakeBase> FireCamShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		UNiagaraSystem* TracerNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		TMap<TEnumAsByte<EPhysicalSurface>, FImpactInfo> arrayPhysicalSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		int currAmmo;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		USoundBase* weaponDryClick;
};
