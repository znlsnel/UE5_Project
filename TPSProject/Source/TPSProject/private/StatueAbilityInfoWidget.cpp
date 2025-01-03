﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueAbilityInfoWidget.h"

void UStatueAbilityInfoWidget::InitWidget(FStatueAbility* ability)
{
	AbilityName = ability->abilityName;
	AbilityInfo = ability->abilityInfo;
	Cost = FString("Cost : ") + FString::Printf(TEXT("%d"), ability->cost);
	Value = "";

	if (ability->abilityType == AbilityType::SealRepair) {
		Value = FString::Printf(TEXT("Repair: %d"), ability->point * 5) + "%";
	} 
	else if (ability->abilityType == AbilityType::HPUpgrade) {
		Value = FString("HP: ") + FString::Printf(TEXT("%d"), ability->point * 50) + " % ";
	}
}

void UStatueAbilityInfoWidget::OpenWidget()
{
	if (IsInViewport() == false)
		AddToViewport();
	PlayAnimation(OnAnim);
}

void UStatueAbilityInfoWidget::CloseWidget()
{
	if (IsInViewport())
		RemoveFromParent();
}
