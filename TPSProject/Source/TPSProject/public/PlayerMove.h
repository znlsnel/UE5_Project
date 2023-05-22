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
	void Turn(float value);		// �¿� ȸ��
	float turnValue = 0.f;
	void LookUp(float value);			// ���� ȸ��
	float lookUpValue = 0.f;

	// Moving
	void InputHorizontal(float value);	// �¿� �̵�
	void InputVertical(float value);		// ���� �̵�
	void InputJump();					// ����
	void InputRun();

	template<DashType type>
	void DoubleClick() { DoubleClick(type); }


	void DoubleClick(DashType dashType);

	void Dash(DashType dashType);


	bool isDash = false;
	// �̵� ����
	FVector direction = FVector(0, 0, 0);

	double gameTime = 0;
	double DoubleClickStartTime = 0;

	UPROPERTY(EditAnywhere, Category = PlayerSetting) float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = PlayerSetting) float runSpeed = 600;
	class UPlayerAnim* playerAnim;
	DashType tempDashType = DashType::None;

	UPROPERTY(EditAnywhere, Category = Jump)
		class USoundBase* jumpSound;
};
