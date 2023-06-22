// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUpgradeWidget.h"
#include "TPSPlayer.h"
#include "PlayerAbilityComp.h"
#include "AbilityInfo.h"

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
