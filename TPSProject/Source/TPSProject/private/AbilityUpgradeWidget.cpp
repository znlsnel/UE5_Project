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
		//tempSkill->UpdateValues(myPlayer);
		UpdateValue(tempSkill);
	}
	else
		UGameplayStatics::PlaySound2D(GetWorld(), failedSound);


}

void UAbilityUpgradeWidget::SkillInfoWidgetEvent(bool isHover, SkillType type)
{
	if (isHover) {
		FSkillInfo* tempInfo = abilityComp->GetSkillInfo(type);

		int nextValue = tempInfo->point < 5 ? tempInfo->powerValues[tempInfo->point] : 0;
		abliltyInfo->InitWidget(tempInfo->SkillName, tempInfo->SkillInfo, tempInfo->powerValue, nextValue);

		double mouseX = 0;
		double mouseY = 0;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
		abliltyInfo->SetPositionInViewport(FVector2D(mouseX, mouseY + 50));

	}
	else
		abliltyInfo->UnHoverWidget();
}


void UAbilityUpgradeWidget::UpdateValue(FSkillInfo* skillInfo)
{
	if (skillInfo->point <= 0) return;

	if (skillInfo->powerValues.Num() >= skillInfo->point)
		skillInfo->powerValue = skillInfo->powerValues[skillInfo->point - 1];
	if (skillInfo->coolDownValues.Num() >= skillInfo->point)
		skillInfo->coolDownValue = skillInfo->coolDownValues[skillInfo->point - 1];

	if (myPlayer && skillInfo->skillType == SkillType::HpUpgrade) {
		myPlayer->UpgradeHp(skillInfo->powerValue);

	}
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
