// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"

#include "PlayerFire.generated.h"


/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

		UPlayerFire();
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

public:
	template<bool b>
	void InputFire() { InputFire(b); }
	void InputFire(bool b);

	void LoopFire();

	void EquipSecondaryWeapon();					// 보조무기 장착
	void EquipPrimaryWeapon();					// 주무기 장착
	template<bool b>
	void SniperAim() { SniperAim(b); }					// 스코프 모드
	void SniperAim(bool isPressed);					// 스코프 모드
	void InitializeWeapon();
	void LoadBullet();
	class AWeapon* GetWeapon();
	void EquipWeapon(WeaponSlotType slotType);
	UFUNCTION(BlueprintCallable)
		void ChangeWeapon();


public:
	bool bSniperAim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUsingPistol = true;

	bool isFire = false;

	FTimerHandle fireTimerHandle;

	class UPlayerAnim* anim;

	float lastShotTime = 0.f;
	// 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		WeaponType currWeapon = WeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = weaponTypes)
		WeaponSlotType currSlot = WeaponSlotType::SecondarySlot;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> pistol;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> rifle;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> shotgun;

	class AWeapon* primaryWeapon;
	class AWeapon* secondaryWeapon;

	// 카메라 쉐이크
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;


	// 총알 Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory; // 총알 공장

	// 총알 이펙트 Particle
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;

	// 총소리
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;



	UPROPERTY(BlueprintReadOnly)
		FVector LeftHandlePos;

	class APlayerController* controller;
};
