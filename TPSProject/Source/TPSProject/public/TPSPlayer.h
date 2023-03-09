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
	// 이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 600;

	// 이동 방향
	FVector direction = FVector(0,0,0);

	void Move();
	UPROPERTY(VisibleAnywhere, category = GunMesh)
		class USkeletalMeshComponent* gunMeshComp;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* tpsCamComp;
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory; // 총알 공장
};
