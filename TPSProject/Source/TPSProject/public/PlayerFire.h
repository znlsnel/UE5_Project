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
	void InputFire();					// 총알 발사
	void GetPistol();					// 권총 장착
	void GetRiple();					// 소총 장착
	void SniperAim();					// 스코프 모드

	bool bUsingPistolGun = true;
	bool bSniperAim = false;

public:


	// 카메라 쉐이크
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;



	// 스코프 조준 UI Widget
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> sniperUIFactory;
	class UUserWidget* _sniperUI;

	// 일반 조준 크로스헤어 UI 위젯
	UPROPERTY(EditDeFaultsOnly, Category = CrosshairUIFactory)
		TSubclassOf<class UUserWidget> crosshairUIFactory;
	class UUserWidget* _crosshairUI;



	// 총알 Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory; // 총알 공장

	// 총알 이펙트 Particle
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;

	// 총소리
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;

};
