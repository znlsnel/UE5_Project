// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnim.h"
#include "PlayerFire.h"
#include "Weapon.h"
#include "TPSPlayer.h"
#include "PlayerAbilityComp.h"
#include "Weapon_Sword.h"

#include <Net/UnrealNetwork.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Animation/AnimSequence.h>


UPlayerAnim::UPlayerAnim()
{

}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (!player)
	{
		auto ownerPawn = TryGetPawnOwner();
		player = Cast<ATPSPlayer>(ownerPawn);

	}


	if (player)
	{
		// 앞뒤 방향 체크
		FVector velocity = player->GetVelocity();


		FVector forwardVector = player->GetActorForwardVector();

		speed = FVector::DotProduct(forwardVector, velocity);

		// 좌우 방향 체크
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		// 떨어지는중인지 체크
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();



		UpdateTurn();
		UpdateTurnAnimation();

	}

}

void UPlayerAnim::PlayMontage(UAnimMontage* animMontage)
{
	if (player->GetNetMode() != NM_DedicatedServer) return;



	Montage_Play(animMontage);
}


void UPlayerAnim::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UPlayerAnim, speed);
}

void UPlayerAnim::UpdateTurn()
{

	pitch = player->GetBaseAimRotation().Pitch;

	if (pitch > 260) pitch -= 360;
	if (speed != 0)
	{
		characterYaw = player->GetActorRotation().Yaw;
		prevCharacterYaw = characterYaw;

		rootYawOffset = 0.f;
	}

	prevCharacterYaw = characterYaw;
	characterYaw = player->GetActorRotation().Yaw;
	float tempYawOffset = rootYawOffset + (prevCharacterYaw - characterYaw);

	rootYawOffset = UKismetMathLibrary::NormalizeAxis(tempYawOffset);

}

void UPlayerAnim::UpdateTurnAnimation()
{

	if (GetCurveValue(TEXT("Turning")))
	{
		prevRotationCurve = rotationCurve;
		rotationCurve = GetCurveValue(TEXT("Rotation"));

		float tempYawOffset = rootYawOffset + (prevRotationCurve - rotationCurve);

		rootYawOffset = UKismetMathLibrary::Clamp(tempYawOffset, -90, 90);
	}

}

void UPlayerAnim::DieEvent()
{
	PlayMontage(DieAnimMontage);
}

void UPlayerAnim::AnimNotify_SkillTrigger()
{
	player->abilityComp->SkillTrigger();
}

void UPlayerAnim::AnimNotify_EndSkill()
{

}

void UPlayerAnim::AnimNotify_OnMeleeAttack()
{
	AWeapon* tempWeapon = player->playerFire->currWeapon;
	if (tempWeapon->weaponType != WeaponType::Sword)
		return;

	
	player->meleeAttackSensor->GetOverlappingActors(Cast<AWeapon_Sword>(tempWeapon)->overlappingActors);

	Cast<AWeapon_Sword>(tempWeapon)->AttackEvent();
	
}

void UPlayerAnim::AnimNotify_Blocking()
{
	Cast<AWeapon_Sword>(player->playerFire->weapon_Sword)->OnBlocking(true);
}

void UPlayerAnim::AnimNotify_BlockingEnd()
{
	Cast<AWeapon_Sword>(player->playerFire->weapon_Sword)->OnBlocking(false);
}

void UPlayerAnim::PlayDashAnim(FVector dir)
{
	UAnimMontage* tempMontage = FowardDashMontage;


	float absX = FMath::Abs(dir.X);
	float absY = FMath::Abs(dir.Y);

	if (absX <= absY) {
		if (dir.Y > 0.f)
			tempMontage = RightDashMontage;
		else 
			tempMontage = LeftDashMontage;
	}
	else {
		if (dir.X > 0.f)
			tempMontage = FowardDashMontage;
		else
			tempMontage = BackwardDashMontage;
	}

	Montage_Play(tempMontage);

	return;
}

