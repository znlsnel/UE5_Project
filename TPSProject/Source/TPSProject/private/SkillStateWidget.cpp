// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStateWidget.h"
#include "AbilityUpgradeWidget.h"

void USkillStateWidget::UpdateCurrPoint()
{
	if (parent == nullptr)
		return;

	skillPoint = parent->abilityComp->GetSkillInfo(skillType)->point;
}
