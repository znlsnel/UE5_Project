// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

UPlayerMove::UPlayerMove()
{
	// Tick�Լ��� ȣ��ǵ��� ó��
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	moveComp->MaxWalkSpeed = runSpeed;

}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move();
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	Super::SetupInputBinding(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);

}


void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMove::Move()
{
	// �÷��̾� �̵�
	// GetControlRotation - �÷��̾� ���� ��Ʈ���ϰ� �ִ� ��Ʈ�ѷ��� ������ FRotator Ÿ������ �Ѱ���
	// FTransform���� Transform �ν���Ʈ�� ����
	// TrasnformVector �� Ư���� vector�� local vector�� ��ȯ������
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector(0, 0, 0);
}
void UPlayerMove::InputHorizontal(float value)
{
	direction.Y = value;
}

void UPlayerMove::InputVertical(float value)
{
	direction.X = value;
}

void UPlayerMove::InputJump()
{
	if (me->GetCharacterMovement()->IsFalling() == false)
		me->Jump();
}


void UPlayerMove::InputRun()
{
	auto movement = moveComp;

	// pressed
	if (movement->MaxWalkSpeed >= runSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	// released
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}

}
