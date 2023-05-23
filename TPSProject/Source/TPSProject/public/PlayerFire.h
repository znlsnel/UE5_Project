// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"

#include "PlayerFire.generated.h"


/**
 *
 */
UCLASS(ClassGroup = (PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

		UPlayerFire();
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	virtual void GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps)const;


public:
	template<bool b>
	void InputFire() { InputFire(b); }
	void InputFire(bool b);

	void LoopFire();


	UFUNCTION(Server, Reliable)
		void EquipWeapon(WeaponType weaponType);
	void EquipWeapon_Implementation(WeaponType weaponType);

	UFUNCTION(NetMulticast, Reliable)
		void EquipWeaponMulticast(WeaponType weaponType);
	void EquipWeaponMulticast_Implementation(WeaponType weaponType);


	template<bool b>
	void SniperAim() { SniperAim(b); }					// 스코프 모드
	void SniperAim(bool isPressed);					// 스코프 모드
	void LoadBullet();


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

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> pistol;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> Rifle;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> Shotgun;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> Bow;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> Sword;

	class AWeapon* weapon_Rifle;
	class AWeapon* weapon_Shotgun;
	class AWeapon* weapon_Pistol;
	class AWeapon* weapon_Bow;
	class AWeapon* weapon_Sword;
	class AWeapon* currWeapon;
	class AWeapon* nextWeapon;
	class AWeapon* GetWeapon(WeaponType weaponType);

	void SetWeapon(class AWeapon* weapon, bool equipWeapon = true);

	void SetWeapon(WeaponType weaponType, bool equipWeapon = true);

	// 카메라 쉐이크
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;


	//// 총알 Factory
	//UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	//	TSubclassOf<class ABullet> bulletFactory; // 총알 공장

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
