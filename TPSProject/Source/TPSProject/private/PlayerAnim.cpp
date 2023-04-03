// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"

#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Animation/AnimSequence.h>

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
		// �յ� ���� üũ
		FVector velocity = player->GetVelocity();

		isMoving = velocity.Length() != 0;

		FVector forwardVector = player->GetActorForwardVector();
		speed = FVector::DotProduct(velocity, forwardVector);

		// �¿� ���� üũ
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		// �������������� üũ
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();



		UpdateTurn();
		UpdateTurnAnimation();
	}

}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}

void UPlayerAnim::UpdateTurn()
{
	pitch = player->GetBaseAimRotation().Pitch;
	
	if (speed > 0)
	{
		characterYaw = player->GetActorRotation().Yaw;
		prevCharacterYaw = characterYaw;
		rootYawOffset = 0.0f;

		return;
	}

	prevCharacterYaw = characterYaw;
	characterYaw = player->GetActorRotation().Yaw;
	float tempYawOffset = rootYawOffset + (prevCharacterYaw - characterYaw);
	rootYawOffset = UKismetMathLibrary::NormalizeAxis(tempYawOffset);
}

void UPlayerAnim::UpdateTurnAnimation()
{
	if (GetCurveValue(TEXT("isTurning")))
	{
		prevRotationCurve = rotationCurve;
		rotationCurve = GetCurveValue(TEXT("Rotation"));
		float tempYawOffset = rootYawOffset + (rotationCurve - prevRotationCurve);
		rootYawOffset = UKismetMathLibrary::Clamp(-90, tempYawOffset, 90);
		
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("RootYawOffset : %f"), rootYawOffset));
	}

	
}


