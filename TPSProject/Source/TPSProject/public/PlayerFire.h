// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void InputFire();					// �Ѿ� �߻�
	void GetPistol();					// ���� ����
	void GetRiple();					// ���� ����
	void SniperAim();					// ������ ���

	bool bUsingPistolGun = true;
	bool bSniperAim = false;

public:


	// ī�޶� ����ũ
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;



	// ������ ���� UI Widget
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> sniperUIFactory;
	class UUserWidget* _sniperUI;

	// �Ϲ� ���� ũ�ν���� UI ����
	UPROPERTY(EditDeFaultsOnly, Category = CrosshairUIFactory)
		TSubclassOf<class UUserWidget> crosshairUIFactory;
	class UUserWidget* _crosshairUI;



	// �Ѿ� Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory; // �Ѿ� ����

	// �Ѿ� ����Ʈ Particle
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;

	// �ѼҸ�
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;

};
