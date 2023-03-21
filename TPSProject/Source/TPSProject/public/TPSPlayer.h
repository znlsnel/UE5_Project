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
	void Turn(float value);				// �¿� ȸ��
	void LookUp(float value);			// ���� ȸ��
	void InputHorizontal(float value);	// �¿� �̵�
	void InputVertical(float value);		// ���� �̵�
	void InputJump();					// ����
	void InputFire();					// �Ѿ� �߻�
	void GetPistol();					// ���� ����
	void GetRiple();					// ���� ����
	void SniperAim();					// ������ ���
	void InputRun();
	void PickUp();
	void PickDown();

public:



	// �̵� ����
	FVector direction = FVector(0,0,0);
	bool bUsingPistolGun = true;
	bool bSniperAim = false;
	bool bisPickUpZone = false;
	bool bPickingUp = false;

	UPROPERTY(EditAnywhere, Category = PlayerSetting) float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = PlayerSetting) float runSpeed = 600;

public:
	// �������� Comp
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;		// Spring Arm

	// ī�޶� Comp
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;			// Camera

	// ���� Mesh
	UPROPERTY(VisibleAnywhere, category = GunMesh)
		class USkeletalMeshComponent* pistolMeshComp;	// Pistol

	// ���� Mesh
	UPROPERTY(VisibleAnywhere, category = GunMesh)
		class UStaticMeshComponent* ripleMeshComp;	// Sniper

	// �Ѿ� ����Ʈ Particle
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;
	class UUserWidget* _sniperUI;


	// �Ѿ� Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory; // �Ѿ� ����

	// ������ ���� UI Widget
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> sniperUIFactory;

	// �Ϲ� ���� ũ�ν���� UI ����
	UPROPERTY(EditDeFaultsOnly, Category = CrosshairUIFactory)
		TSubclassOf<class UUserWidget> crosshairUIFactory;
	// ũ�ν���� �ν��Ͻ�
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
