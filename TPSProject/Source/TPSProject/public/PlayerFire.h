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
	void SniperAim() { SniperAim(b); }					// ������ ���
	void SniperAim(bool isPressed);					// ������ ���
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
	// ����

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

	// ī�޶� ����ũ
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;


	//// �Ѿ� Factory
	//UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	//	TSubclassOf<class ABullet> bulletFactory; // �Ѿ� ����

	// �Ѿ� ����Ʈ Particle
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;

	// �ѼҸ�
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;



	UPROPERTY(BlueprintReadOnly)
		FVector LeftHandlePos;

	class APlayerController* controller;
};
