// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUpgradeWidget.h"
#include "TPSPlayer.h"
#include "PlayerAbilityComp.h"
#include "AbilityInfo.h"
#include "SkillStateWidget.h"

#include <Kismet/GameplayStatics.h>


void UAbilityUpgradeWidget::NativeConstruct()
{
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (myPlayer)
		abilityComp = myPlayer->abilityComp;

	abliltyInfo = CreateWidget<UAbilityInfo>(GetWorld(), AbilityInfoFactory);
	InitWidget();
}

void UAbilityUpgradeWidget::UpgradeSkill(SkillType skillType, bool& result)
{
	result = true;
	FSkillInfo* tempSkill = abilityComp->GetSkillInfo(skillType);
	if (tempSkill == nullptr) {
		result = false;
		return;
	}

	if (currSkillCoin == 0 || tempSkill->point >= 5) {
		result = false;
	}
	
	switch (tempSkill->SectionType)
	{
	case SkillSectionType::secondSkillSection: {
		if (RemainingSecondSkillCount > 0) {
			result = false;
		}
	}
		break;
	case SkillSectionType::thirdSkillSection: {
		if (RemainingThirdSkillCount > 0) {
			result = false;
		}
	}
		break;
	case SkillSectionType::fourthSkillSection: {
		if (RemainingFourthSkillCount > 0) {
			result = false;
		}
	}
		break;
	}



	if (result) {
		UGameplayStatics::PlaySound2D(GetWorld(), successSound);
		currSkillCoin--;
		tempSkill->point++;

		switch (tempSkill->SectionType)
		{
		case SkillSectionType::firstSkillSection:
			used_FirstSection++;
			break;
		case SkillSectionType::secondSkillSection: {
			used_SecondSection++;
		}
			break;
		case SkillSectionType::thirdSkillSection: {
			used_ThirdSection++;
		}
			break;
		}

		UpdateRemainSkillCount();
		abilityComp->UpdateValue(tempSkill);

		abliltyInfo->InitWidget(tempSkill, false);
	}
	else
		UGameplayStatics::PlaySound2D(GetWorld(), failedSound);


}

void UAbilityUpgradeWidget::SkillInfoWidgetEvent(bool isHover, SkillType type, FVector2D pos)
{
	if (isHover) {
		FSkillInfo* tempInfo = abilityComp->GetSkillInfo(type);

	
		abliltyInfo->InitWidget(tempInfo);
		abliltyInfo->SetPositionInViewport(pos);
	}
	else
		abliltyInfo->UnHoverWidget();
}



void UAbilityUpgradeWidget::OpenWidget()
{
	if (IsAnimationPlaying(OpenWidgetAnim) || IsAnimationPlaying(CloseWidgetAnim))
		return;
	
	AddToViewport();

	PlayAnimation(OpenWidgetAnim);
	isOpen = true;
}

void UAbilityUpgradeWidget::CloseWidget()
{
	if (IsAnimationPlaying(OpenWidgetAnim) || IsAnimationPlaying(CloseWidgetAnim))
		return;

	PlayAnimation(CloseWidgetAnim);
	abliltyInfo->UnHoverWidget();

	GetWorld()->GetTimerManager().SetTimer(closeWidgetTimer, FTimerDelegate::CreateLambda(
		[&]() {
			if(IsInViewport())
				RemoveFromParent();
			isOpen = false;
		}
	), 1.f, false);
}

void UAbilityUpgradeWidget::UpdateRemainSkillCount()
{


	RemainingSecondSkillCount = Unlock_SecondSection - used_FirstSection;

	RemainingThirdSkillCount = RemainingSecondSkillCount + Unlock_ThirdSection - used_SecondSection;

	RemainingFourthSkillCount = RemainingThirdSkillCount + Unlock_FourthSection - used_ThirdSection;


	toSecondSectionRate = static_cast<float>(Unlock_SecondSection - RemainingSecondSkillCount) / static_cast<float>(Unlock_SecondSection);

	if (RemainingThirdSkillCount > Unlock_ThirdSection + Unlock_ThirdSection)
		toThirdSectionRate = 0.f;
	else {
		toThirdSectionRate = static_cast<float>(Unlock_ThirdSection - RemainingThirdSkillCount) / static_cast<float>(Unlock_ThirdSection);
	}

	if (RemainingFourthSkillCount > Unlock_FourthSection + Unlock_ThirdSection + Unlock_ThirdSection)
		toFourthSectionRate = 0.f;
	else {
		toFourthSectionRate = static_cast<float>(Unlock_FourthSection - RemainingFourthSkillCount) / static_cast<float>(Unlock_FourthSection);
	}

	playSkillSectionUIAnimation();
	return;
}

FUpgradeInfo UAbilityUpgradeWidget::GetAbilityInfo()
{
	FUpgradeInfo result;
	result.currSkillPoint = currSkillCoin;

	result.Unlock_SecondSection = Unlock_SecondSection;
	result.Unlock_ThirdSection = Unlock_ThirdSection;
	result.Unlock_FourthSection = Unlock_FourthSection;

	result.used_FirstSection = used_FirstSection;
	result.used_SecondSection = used_SecondSection;
	result.used_ThirdSection = used_ThirdSection;

	result.RemainingSecondSkillCount = RemainingSecondSkillCount;
	result.RemainingThirdSkillCount = RemainingThirdSkillCount;
	result.RemainingFourthSkillCount = RemainingFourthSkillCount;

	result.toSecondSectionRate = toSecondSectionRate;
	result.toThirdSectionRate = toThirdSectionRate;
	result.toFourthSectionRate = toFourthSectionRate;


	return result;
}

void UAbilityUpgradeWidget::SetAbilityInfo(FUpgradeInfo upgradeInfo)
{
	currSkillCoin = upgradeInfo.currSkillPoint;

	Unlock_SecondSection = upgradeInfo.Unlock_SecondSection;
	Unlock_ThirdSection = upgradeInfo.Unlock_ThirdSection;
	Unlock_FourthSection = upgradeInfo.Unlock_FourthSection;
	
	used_FirstSection =upgradeInfo.used_FirstSection;
	used_SecondSection = upgradeInfo.used_SecondSection;
	used_ThirdSection =upgradeInfo. used_ThirdSection;
	
	RemainingSecondSkillCount = upgradeInfo.RemainingSecondSkillCount;
	RemainingThirdSkillCount = upgradeInfo.RemainingThirdSkillCount;
	RemainingFourthSkillCount = upgradeInfo.RemainingFourthSkillCount;
	
	toSecondSectionRate =upgradeInfo. toSecondSectionRate;
	toThirdSectionRate = upgradeInfo.toThirdSectionRate;
	toFourthSectionRate =upgradeInfo. toFourthSectionRate;

	for (auto skill : skillButtons) {
		skill->UpdateCurrPoint();
	}
}
