// Fill out your copyright notice in the Description page of Project Settings.


#include "RiktorAnim.h"
#include "Riktor.h"


void URiktorAnim::LongRangeAttack()
{
	PlayAttackAnim(true);
}

void URiktorAnim::AnimNotify_OnRangeIndicator()
{
	Cast<ARiktor>(me)->OnAttackRangeIndicator(false);
}

void URiktorAnim::AnimNotify_TriggerRangeIndicator()
{
	Cast<ARiktor>(me)->OnAttackRangeIndicator(true);

}

void URiktorAnim::AnimNotify_SpawnThunderMonster()
{
	Cast<ARiktor>(me)->SpawnThunderMonster();
}
