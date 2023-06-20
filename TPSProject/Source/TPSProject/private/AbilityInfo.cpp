// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInfo.h"

void UAbilityInfo::InitWidget(FString skillName, FString skillInfomation, int currValue, int nextValue)
{
	SkillName = skillName;
	SkillInfomation = skillInfomation;

	CurrValue = FString::Printf(TEXT("current : %d"), currValue);
	NextValue = FString::Printf(TEXT("next : %d"), nextValue);

	if (currValue == 0)
		CurrValue = "";
	if (nextValue == 0)
		NextValue = "";

	HoverWidget();
}
