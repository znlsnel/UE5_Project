// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayPlayerMontage(class UAnimMontage* animMontage);
	
	void UpdateTurn();
	void UpdateTurnAnimation();
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		float speed = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		bool isInAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		float direction = 0;

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* attackAnimMontage;
	
	UPROPERTY(blueprintReadOnly, Category = player)
		class ATPSPlayer* player;

	// ROOT BONE ROTATION
	UPROPERTY(blueprintReadOnly, category = RootBoneRotate)
		float pitch = 0.f;
	UPROPERTY(blueprintReadOnly, category = RootBoneRotate)
		float rootYawOffset = 0.f;
	UPROPERTY(blueprintReadOnly, category = RootBoneRotate)
		float characterYaw = 0.f;
	UPROPERTY(blueprintReadOnly, category = RootBoneRotate)
		float prevCharacterYaw = 0.f;
	UPROPERTY(blueprintReadOnly, category = RootBoneRotate)
		float prevRotationCurve = 0.f;
	UPROPERTY(blueprintReadOnly, category = RootBoneRotate)
		float rotationCurve = 0.f;
	

};
