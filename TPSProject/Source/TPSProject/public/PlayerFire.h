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

	void EquipSecondaryWeapon();					// �������� ����
	void EquipPrimaryWeapon();					// �ֹ��� ����
	template<bool b>
	void SniperAim() { SniperAim(b); }					// ������ ���
	void SniperAim(bool isPressed);					// ������ ���
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
	// ����
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

	// ī�޶� ����ũ
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;


	// �Ѿ� Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory; // �Ѿ� ����

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
