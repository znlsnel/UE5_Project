// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilityComp.h"

// Sets default values for this component's properties
UPlayerAbilityComp::UPlayerAbilityComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	{
		swordProficiencyPoint = FSkillInfo(SkillSectionType::firstSkillSection, SkillType::swordProficiency, true, { 10, 25, 50, 70, 100 }, {});
		bowProficiencyPoint = FSkillInfo(SkillSectionType::thirdSkillSection, SkillType::bowProficiency, true, { 5, 12, 25, 40, 45 }, {});
		gunProficiencyPoint = FSkillInfo(SkillSectionType::secondSkillSection, SkillType::gunProficiency, true, { 2, 6, 10, 17, 25 }, {});
		HpUpgradePoint = FSkillInfo(SkillSectionType::firstSkillSection, SkillType::HpUpgrade, true, { 10, 25, 50, 70, 100 }, {});
		HpNaturalHealingPoint = FSkillInfo(SkillSectionType::secondSkillSection, SkillType::HpNaturalHealing, true, { 2, 4, 6, 8, 10 }, {});
		DoubleAttackPoint = FSkillInfo(SkillSectionType::thirdSkillSection, SkillType::DoubleAttack, true, { 10, 20, 40, 60, 80 }, {});
		LuckyShotPoint = FSkillInfo(SkillSectionType::fourthSkillSection, SkillType::LuckyShot, true, { 15, 25, 42, 70, 100 }, {});
		CoinBoostPoint = FSkillInfo(SkillSectionType::fourthSkillSection, SkillType::CoinBoost, true, { 120, 140, 180, 250, 300 }, {});

		IceAttackPoint = FSkillInfo(SkillSectionType::firstSkillSection, SkillType::IceAttack, false, { 30, 50, 70, 100 }, { 5.f, 4.f, 3.f, 2.f, 1.f });
		LightningStrikePoint = FSkillInfo(SkillSectionType::secondSkillSection, SkillType::LightningStrike, false, { 50, 80, 110, 150, 200 }, { 10.f, 7.5f, 5.f, 3.5f, 2.f });
		HealingPoint = FSkillInfo(SkillSectionType::thirdSkillSection, SkillType::Healing, false, { 10, 17, 23, 30, 45 }, { 15.f, 12.f, 10.f, 8.f, 6.f });
		FireStormPoint = FSkillInfo(SkillSectionType::fourthSkillSection, SkillType::FireStorm, false, { 100, 200, 300, 400, 500 }, { 20.f, 17.f, 13.f, 10.f, 8.f });

		skillInfos.Add(&swordProficiencyPoint);
		skillInfos.Add(&bowProficiencyPoint);
		skillInfos.Add(&gunProficiencyPoint);
		skillInfos.Add(&HpUpgradePoint);
		skillInfos.Add(&HpNaturalHealingPoint);
		skillInfos.Add(&DoubleAttackPoint);
		skillInfos.Add(&LuckyShotPoint);
		skillInfos.Add(&CoinBoostPoint);
		skillInfos.Add(&IceAttackPoint);
		skillInfos.Add(&LightningStrikePoint);
		skillInfos.Add(&HealingPoint);
		skillInfos.Add(&FireStormPoint);
	}
	// ...
}


// Called when the game starts
void UPlayerAbilityComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FSkillInfo* UPlayerAbilityComp::GetSkillInfo(SkillType type)
{
	for (auto skillInfo : skillInfos)
	{

		if (skillInfo->skillType == type)
			return skillInfo;
	}

	return nullptr;
}

