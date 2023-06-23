// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInfo.h"

#include <Kismet/KismetMathLibrary.h>

void UAbilityInfo::InitWidget(FSkillInfo* skillInfo, bool StartHover)
{
	//FString skillName, FString skillInfomation, int currValue, int nextValue, float currCoolTime, float nextCoolTime, bool StartHover
	if (IsInViewport() == false)
		AddToViewport();
	SkillName = skillInfo->SkillName;
	SkillInfomation = skillInfo->SkillInfo;

	int currValue = skillInfo->powerValue;
	int nextValue = skillInfo->point < 5 ? skillInfo->powerValues[skillInfo->point] : 0;

	CurrValue = FString::Printf(TEXT("current : %d"), currValue);
	NextValue = FString::Printf(TEXT("next : %d"), nextValue);

	float currCoolTime = skillInfo->coolDownValue;
	float nextCoolTime = 0.f;
	if (skillInfo->coolDownValues.IsEmpty() == false && skillInfo->point < 5)
		nextCoolTime = skillInfo->coolDownValues[skillInfo->point];

	FString currCT = FText::AsNumber(currCoolTime, &FNumberFormattingOptions().SetMinimumFractionalDigits(1)).ToString();
	FString NextCT = " / " + FText::AsNumber(nextCoolTime, &FNumberFormattingOptions().SetMinimumFractionalDigits(1)).ToString();
	if (currCoolTime == 0.f) currCT = "None";
	if (nextCoolTime == 0.f) NextCT = "";

	CoolTimeText = "CoolTime : " + currCT  + NextCT;

	if (currCoolTime == 0.f && nextCoolTime == 0.f)
		CoolTimeText = "";
	
	if (currValue == 0) CurrValue = "";
	if (nextValue == 0) NextValue = "";

	if (StartHover)
		HoverWidget();
}
