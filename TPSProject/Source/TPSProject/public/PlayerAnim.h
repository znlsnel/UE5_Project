// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
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

	void PlayMontage(class UAnimMontage* animMontage);

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	void UpdateTurn();
	void UpdateTurnAnimation();
	void DieEvent();

	UFUNCTION()
		void AnimNotify_SkillTrigger();
	UFUNCTION()
		void AnimNotify_EndSkill();
	UFUNCTION()
		void AnimNotify_EndSwing();
	//UFUNCTION(BlueprintImplementableEvent)

	UAnimMontage* Dash(DashType dashDirection);



public:
	UPROPERTY(Replicated)
		class UAnimMontage* currMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = PlayerAnim)
		float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		bool isInAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		float direction = 0;

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* attackAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DieAnimMontage;
	UPROPERTY(blueprintReadOnly, Category = player)
		class ATPSPlayer* player;

	UPROPERTY(BlueprintReadOnly = Category = Weapon)
		WeaponType weaponType = WeaponType::Pistol;
	UPROPERTY(BlueprintReadWrite = Category = Weapon)
		BowState bowState = BowState::TravelMode;

	UPROPERTY(EditAnywhere, Category = DashMonstage)
		class UAnimMontage* FowardDashMontage;
	UPROPERTY(EditAnywhere, Category = DashMonstage)
		class UAnimMontage* BackwardDashMontage;
	UPROPERTY(EditAnywhere, Category = DashMonstage)
		class UAnimMontage* LeftDashMontage;
	UPROPERTY(EditAnywhere, Category = DashMonstage)
		class UAnimMontage* RightDashMontage;

	// ROOT BONE ROTATION
	UPROPERTY(blueprintReadOnly, Replicated, category = RootBoneRotate)
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
