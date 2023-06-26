// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "PlayerUI.h"
#include "PlayerAnim.h"
#include "PlayerFire.h"
#include "PlayerAbilityComp.h"
#include "Weapon.h"
#include "Weapon_Sword.h"
#include "ScreenUI.h"

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

	if (isDash) {
		me->AddActorWorldOffset(dashDir * 10);
	}
	else {
		Move();
	}
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

	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &UPlayerMove::Dash);

}

#include "BuildableItem.h"
void UPlayerMove::Turn(float value)
{

	if (me->isDie) return;
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
	if (me->isDie) return;
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
	if (me->isDie) return;
	// 플레이어 이동
	// GetControlRotation - 플레이어 폰을 컨트롤하고 있는 컨트롤러의 방향을 FRotator 타입으로 넘겨줌
	// FTransform으로 Transform 인스턴트를 생성
	// TrasnformVector 는 특정한 vector를 local vector로 변환시켜줌
	if (me->abilityComp->isPlaySkillAnim) 
		return;

	if (me->playerFire->currWeapon && 
		me->playerFire->currWeapon->ActorHasTag(TEXT("Sword")) &&
		Cast<AWeapon_Sword>(me->playerFire->currWeapon)->IsActiveSword) 
		return;


	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);

	direction = FVector(0, 0, 0);
}
void UPlayerMove::InputHorizontal(float value)
{
	direction.Y = value;
	if (isDash == false)
		dashDir.Y = value;
}

void UPlayerMove::InputVertical(float value)
{
	direction.X = value;

	if (isDash == false)
		dashDir.X = value;
}

void UPlayerMove::InputJump()
{
	if (me->playerFire->currWeapon && me->playerFire->currWeapon->weaponType == WeaponType::Sword) {
		if (Cast<AWeapon_Sword>(me->playerFire->currWeapon)->isBlocking == true)
			return;
	}

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


void UPlayerMove::Dash( )
{
	if (GetWorld()->GetTimeSeconds() - lastDashTime < dashCoolTime)
		return;

	lastDashTime = GetWorld()->GetTimeSeconds();
	playerAnim->PlayDashAnim(dashDir);

	isDash = true;

	float absX = FMath::Abs(dashDir.X);
	float absY = FMath::Abs(dashDir.Y);

	if (dashDir.X == 0 && dashDir.Y == 0)
		dashDir = me->GetActorForwardVector();


	else if (absX < absY) {
		if (dashDir.Y > 0.f)
			dashDir = me->GetActorRightVector();
		else
			dashDir = -(me->GetActorRightVector());

	}
	else {
		if (dashDir.X > 0.f)
			dashDir = me->GetActorForwardVector();
		else
			dashDir = -(me->GetActorForwardVector());

	}
	
	me->playerUI->screenUI->DashTime = dashCoolTime;
	me->playerUI->screenUI->DashTimeText = FString::Printf(TEXT("%d"), (int)dashCoolTime);


	if (GetWorld()->GetTimerManager().IsTimerActive(me->abilityComp->skillTimerHandle) == false)
		me->abilityComp->OperateSkillTimer();

	GetWorld()->GetTimerManager().ClearTimer(dashTimer);
	GetWorld()->GetTimerManager().SetTimer(dashTimer, FTimerDelegate::CreateLambda(
		[&]() {
			isDash = false;
		}), 0.7f, false);

	return;
}

