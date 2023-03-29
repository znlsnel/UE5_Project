// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void Turn(float value);				// �¿� ȸ��
	void LookUp(float value);			// ���� ȸ��

	// Moving
	void InputHorizontal(float value);	// �¿� �̵�
	void InputVertical(float value);		// ���� �̵�
	void InputJump();					// ����
	void InputRun();

	// �̵� ����
	FVector direction = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, Category = PlayerSetting) float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = PlayerSetting) float runSpeed = 600;

};
