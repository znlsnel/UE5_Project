// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSPlayer.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAbilityComp.generated.h"


UENUM(BlueprintType)
enum class SkillSectionType : uint8
{
	firstSkillSection,
	secondSkillSection,
	thirdSkillSection,
	fourthSkillSection,
	None,
};

UENUM(BlueprintType)
enum class SkillType : uint8
{
	swordProficiency,
	bowProficiency,
	gunProficiency,
	HpUpgrade,
	HpNaturalHealing,
	DoubleAttack,
	LuckyShot,
	CoinBoost,
	IceAttack,
	LightningStrike,
	Healing,
	FireStorm,
	None
};


USTRUCT(BlueprintType)
struct TPSPROJECT_API FSkillInfo
{
	GENERATED_BODY()
public:
	FSkillInfo() {};
	// Array俊 5俺究 持绢辑 积己
	FSkillInfo(SkillSectionType type, SkillType skill, bool isAbilitySkill, TArray<int> power, TArray<float> coolDown) {
		attackPower = power;
		skillcoolDown = coolDown;
		SectionType = type;
		isAbility = isAbilitySkill;
		skillType = skill;
	};
	void UpdateValues(class ATPSPlayer* player = nullptr) {
		if (point <= 0) return;

		if (attackPower.Num() >= point)
			powerValue = attackPower[point - 1];
		if (skillcoolDown.Num() >= point)
			coolDownValue = skillcoolDown[point - 1];

		if (player && skillType == SkillType::HpUpgrade){
			player->UpgradeHp(powerValue);

		}
	};
	UPROPERTY(BlueprintReadOnly)
		TArray<int> attackPower;

	UPROPERTY(BlueprintReadOnly)
		TArray<float> skillcoolDown;

	UPROPERTY(BlueprintReadOnly)
		int isAbility = false;

	UPROPERTY(BlueprintReadOnly)
		int point = 0;

	int powerValue = 0;
	int coolDownValue = 0;

	UPROPERTY(BlueprintReadOnly)
		SkillSectionType SectionType = SkillSectionType::None;

	UPROPERTY(BlueprintReadOnly)
		SkillType skillType = SkillType::None;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerAbilityComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAbilityComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FSkillInfo* GetSkillInfo(SkillType type);

public:
	class ATPSPlayer* myPlayer;
		

	UPROPERTY(BlueprintReadWrite)
		int skillPoint = 0;

	TArray<FSkillInfo*> skillInfos;
	

	UPROPERTY(BlueprintReadOnly)
		FSkillInfo swordProficiencyPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo bowProficiencyPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo gunProficiencyPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo HpUpgradePoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo HpNaturalHealingPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo DoubleAttackPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo LuckyShotPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo CoinBoostPoint;

	UPROPERTY(BlueprintReadOnly)
		FSkillInfo IceAttackPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo LightningStrikePoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo HealingPoint;
	UPROPERTY(BlueprintReadOnly)
		FSkillInfo FireStormPoint;


};
