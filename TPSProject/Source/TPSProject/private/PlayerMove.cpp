// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "PlayerUI.h"
#include "PlayerAnim.h"
#include "PlayerFire.h"
#include "PlayerAbilityComp.h"
#include "Weapon.h"
#include "Weapon_Sword.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
UPlayerMove::UPlayerMove()
{
	// Tick함수가 호출되도록 처리
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	moveComp->MaxWalkSpeed = runSpeed;
	playerAnim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	gameTime += DeltaTime;

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

	PlayerInputComponent->BindAction(TEXT("Dash_W"), IE_DoubleClick, this, &UPlayerMove::DoubleClick<DashType::W>);

	PlayerInputComponent->BindAction(TEXT("Dash_A"), IE_DoubleClick, this, &UPlayerMove::DoubleClick<DashType::A>);

	PlayerInputComponent->BindAction(TEXT("Dash_S"), IE_DoubleClick, this, &UPlayerMove::DoubleClick<DashType::S>);

	PlayerInputComponent->BindAction(TEXT("Dash_D"), IE_DoubleClick, this, &UPlayerMove::DoubleClick<DashType::D>);
}

#include "BuildableItem.h"
void UPlayerMove::Turn(float value)
{

	if (me->abilityComp->isPlaySkillAnim) return;

	turnValue += value;
	turnValue = UKismetMathLibrary::FClamp(turnValue, -10, 10);
	
	ABuildableItem* tempItem = me->buildableItem;
	if (IsValid(tempItem) && tempItem->isBuild && tempItem->isSetLocation == false) {
		tempItem->turnValue = value;
		return;
	}

	if (me->playerUI->IsMouseActive) return;
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(float value)
{
	if (me->abilityComp->isPlaySkillAnim) return;

	lookUpValue += value;
	lookUpValue = UKismetMathLibrary::FClamp(lookUpValue, -10, 10);

	ABuildableItem* tempItem = me->buildableItem;
	if (IsValid(tempItem) && tempItem->isBuild && tempItem->isSetLocation == false) {
		return;
	}

	if (me->playerUI->IsMouseActive) return;

	
	me->AddControllerPitchInput(value);
}

void UPlayerMove::Move()
{
	// 플레이어 이동
	// GetControlRotation - 플레이어 폰을 컨트롤하고 있는 컨트롤러의 방향을 FRotator 타입으로 넘겨줌
	// FTransform으로 Transform 인스턴트를 생성
	// TrasnformVector 는 특정한 vector를 local vector로 변환시켜줌
	if (me->abilityComp->isPlaySkillAnim) 
		return;

	if (me->playerFire->currWeapon && 
		me->playerFire->currWeapon->ActorHasTag(TEXT("Sword")) &&
		Cast<AWeapon_Sword>(me->playerFire->currWeapon)->isSwingingSword) 
		return;

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
	{
		me->Jump();
		UGameplayStatics::PlaySound2D(GetWorld(), jumpSound);
	}
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

void UPlayerMove::DoubleClick(DashType dashDirection)
{
	Dash(dashDirection);
}

void UPlayerMove::Dash(DashType dashDirection)
{
	//UAnimMontage* dashMontage = playerAnim->Dash(dashDirection);

	////playerAnim->PlayMontage(dashMontage);
	//me->PlayMontageInServer(dashMontage);
	//FVector ImpulseDirection;
	//switch (dashDirection)
	//{
	//case DashType::W:
	//	ImpulseDirection = me->GetActorForwardVector();
	//	break;
	//case DashType::A:
	//	ImpulseDirection = -me->GetActorRightVector();
	//	break;
	//case DashType::S:
	//	ImpulseDirection = -me->GetActorForwardVector();
	//	break;
	//case DashType::D:
	//	ImpulseDirection = me->GetActorRightVector();
	//	break;
	//}

	//float ImpulseMagnitude = 70000.0f;
	//FVector Impulse = ImpulseMagnitude * ImpulseDirection;



	//FVector Point = me->GetActorLocation();

	//me->Jump();
	//me->GetCharacterMovement()->AddImpulse(Impulse);

	//return;
}
