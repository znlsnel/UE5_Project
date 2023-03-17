// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 플레이어의 이동 속도를 가져와 speed에 할당
	auto ownerPawn = TryGetPawnOwner();
	// 플레이어로 캐스트
	auto player = Cast<ATPSPlayer>(ownerPawn);

	

	if (player)
	{
		// 앞뒤 방향 체크
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		speed = FVector::DotProduct(velocity, forwardVector);

		// 좌우 방향 체크
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		// 떨어지는중인지 체크
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}
