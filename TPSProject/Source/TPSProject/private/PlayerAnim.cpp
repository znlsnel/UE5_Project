// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �÷��̾��� �̵� �ӵ��� ������ speed�� �Ҵ�
	auto ownerPawn = TryGetPawnOwner();
	// �÷��̾�� ĳ��Ʈ
	auto player = Cast<ATPSPlayer>(ownerPawn);

	

	if (player)
	{
		// �յ� ���� üũ
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		speed = FVector::DotProduct(velocity, forwardVector);

		// �¿� ���� üũ
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		// �������������� üũ
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}
