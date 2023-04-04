// Fill out your copyright notice in the Description page of Project Settings.


#include "FootIkActorComponent.h"
#include "PlayerAnim.h"
#include "TPSPlayer.h"

#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Animation/AnimSequence.h>

UPlayerAnim::UPlayerAnim()
{
	footIkComp = CreateDefaultSubobject<UFootIkActorComponent>(TEXT("FootIKComp"));
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("RootYawOffset : %f"), rootYawOffset));

	if (!player)
	{
		auto ownerPawn = TryGetPawnOwner();
		player = Cast<ATPSPlayer>(ownerPawn);
	}
	

	if (player)
	{
		// �յ� ���� üũ
		FVector velocity = player->GetVelocity();


		FVector forwardVector = player->GetActorForwardVector();

		speed = FVector::DotProduct(forwardVector, velocity);

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

	if (GetCurveValue(TEXT("isTurning")))
	{
		prevRotationCurve = rotationCurve;
		rotationCurve = GetCurveValue(TEXT("Rotation"));

		float tempYawOffset = rootYawOffset + (prevRotationCurve - rotationCurve);

		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT(" tempYawOffset : %f"), tempYawOffset));

		rootYawOffset = UKismetMathLibrary::Clamp(tempYawOffset , -90, 90);

	}

}


