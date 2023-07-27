// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Chaos/ChaosEngineInterface.h>

#include "Item.h"
#include "playerFire.h"
#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "PlayerAbilityComp.h"
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
	TArray<FHitResult> LineTrace();
	void HideWeapon();
	void UncoverWeapon();
	void DiscardWeaponIfAlreadyExists();
	virtual void Reload();
	

	//void createNiagara_Implementation( FHitResult pHitResult);

	//UFUNCTION(NetMulticast, Reliable)
	//	void MulticastNiaga( FHitResult pHitResult);
	//	void MulticastNiaga_Implementation( FHitResult pHitResult);

	UFUNCTION(BlueprintImplementableEvent)
		void SetSync(bool isSync);

	//UFUNCTION(NetMulticast, Reliable)
	//	void ClickWidget(bool isFire);
	//	void ClickWidget_Implementation(bool isFire);
	//	UFUNCTION(NetMulticast, Reliable)
	//		void ClickWidgetMulti(bool isFire);
	//	void ClickWidgetMulti_Implementation(bool isFire);


	UFUNCTION(BlueprintImplementableEvent)
		void ClickWorldWidget(bool isPress);
	FVector MyNormalize(const FVector& Invec);

	void CreateDecal(UNiagaraComponent* tempDecal, FHitResult& pHitResult);
public:
	class AEnemy* tempEnemy;
	TArray<class AEnemy*> tempEnemys;
	FTimerHandle doubleAttackTimer;


	UPlayerAnim* anim;
	bool isSynchronized = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = WeaponMesh)
		class USkeletalMeshComponent* weaponMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		 WeaponType weaponType = WeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		AmmoType ammoType = AmmoType::PistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		WeaponSlotType weaponSlotType = WeaponSlotType::SecondarySlot;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponTypes)
		UTexture2D* ammoIcon;

	// 총을 쏜 후 다시 총을 쏠수 있을 때까지의 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		float fireDelay = 10.f;

	// 한번에 발사되는 총알의 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		int FireBulletCount = 1;

	// 매 사격시 정확도가 낮아지는 정도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		float FireSpread = 0.f;
	float InitFireSpread = 0.f;

	float currFireSpread = 0.f;
	float lastFiredTime = 0.f;


	// 총 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		float weapDamage = 10.f;

	// 총을 장착할 캐릭터의 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponVars)
		FName attachCharacterSocketName = FName("RightHandSocket");
	
		
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ImpactDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ImpactEffect;


};
