// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	UPlayerMove();

public:
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

// Method
	void Move();

// Input System
	// Camera
	void Turn(float value);		// 좌우 회전
	float turnValue = 0.f;
	void LookUp(float value);			// 상하 회전
	float lookUpValue = 0.f;

	// Moving
	void InputHorizontal(float value);	// 좌우 이동
	void InputVertical(float value);		// 상하 이동
	void InputJump();					// 점프
	void InputRun();

	void Dash();

	bool isDash = false;
	// 이동 방향
	FVector direction = FVector(0, 0, 0);
	FVector dashDir = FVector(0);

	float lastDashTime = -3.f;
	float dashCoolTime = 5.f;

	UPROPERTY(EditAnywhere, Category = PlayerSetting) 
		float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = PlayerSetting) 
		float runSpeed = 600;

	class UPlayerAnim* playerAnim;
	UPROPERTY(EditAnywhere, Category = Jump)
		class USoundBase* jumpSound;

	FTimerHandle dashTimer;
};
