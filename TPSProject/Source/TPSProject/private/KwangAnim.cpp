// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangAnim.h"
#include "Kwang.h"

#include "AttackRangeIndicator.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>

void UKwangAnim::PlayAttackAnim( bool isLongRangeAttack, bool startMotion)
{
	//Cast<AKwang>(me)->isLightningLoop = false;

	Super::PlayAttackAnim(isLongRangeAttack);
}

void UKwangAnim::PlayDamageAnim(bool IsDeath, AActor* attacker)
{
	Super::PlayDamageAnim(IsDeath, attacker);

	if (IsDeath && myIndicator) {
		myIndicator->SetActive(false);

	}
}

void UKwangAnim::LongRangeAttack()
{
	PlayAttackAnim(true);
}

void UKwangAnim::AnimNotify_OnAttackRangeIndicator()
{
	myIndicator = Cast<AKwang>(me)->GetAttackRangeIndicator();


	if (myIndicator) {
		myIndicator->SetMode(true);
		myIndicator->SetActive(true, me->GetActorLocation());
	}
}

void UKwangAnim::AnimNotify_OnSkill()
{
	if (myIndicator) {
		myIndicator->SetAttackTimer(me, 0.1f);
	}
	else
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Indicator"));
}

void UKwangAnim::AnimNotify_LightningStrike()
{
	Cast<AKwang>(me)->isLightningLoop = true;
	Cast<AKwang>(me)->LoopLightningStrike();
}

void UKwangAnim::AnimNotify_LightningStrikeEnd()
{
	Cast<AKwang>(me)->isLightningLoop = false;
}
