// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

	void Move();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input System
	void Turn(float value);				// 좌우 회전
	void LookUp(float value);			// 상하 회전
	void InputHorizontal(float value);	// 좌우 이동
	void InputVertical(float value);		// 상하 이동
	void InputJump();					// 점프
	void InputFire();					// 총알 발사
	void GetPistol();					// 권총 장착
	void GetRiple();					// 소총 장착
	void SniperAim();					// 스코프 모드
	void InputRun();
	void PickUp();
	void PickDown();

public:



	// 이동 방향
	FVector direction = FVector(0,0,0);
	bool bUsingPistolGun = true;
	bool bSniperAim = false;
	bool bisPickUpZone = false;
	bool bPickingUp = false;

	UPROPERTY(EditAnywhere, Category = PlayerSetting) float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = PlayerSetting) float runSpeed = 600;

public:
	// 스프링암 Comp
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;		// Spring Arm

	// 카메라 Comp
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;			// Camera

	// 권총 Mesh
	UPROPERTY(VisibleAnywhere, category = GunMesh)
		class USkeletalMeshComponent* pistolMeshComp;	// Pistol

	// 소총 Mesh
	UPROPERTY(VisibleAnywhere, category = GunMesh)
		class UStaticMeshComponent* ripleMeshComp;	// Sniper

	// 총알 이펙트 Particle
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;
	class UUserWidget* _sniperUI;


	// 총알 Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory; // 총알 공장

	// 스코프 조준 UI Widget
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> sniperUIFactory;

	// 일반 조준 크로스헤어 UI 위젯
	UPROPERTY(EditDeFaultsOnly, Category = CrosshairUIFactory)
		TSubclassOf<class UUserWidget> crosshairUIFactory;
	// 크로스헤어 인스턴스
	class UUserWidget* _crosshairUI;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;

	UPROPERTY(BlueprintReadWrite, Category = weaponOpen)
		bool bRipleOpen = false;

	UPROPERTY(BlueprintReadWrite, Category = weaponOpen)
		bool bPistolOpen = true;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);
};
