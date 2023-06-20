// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	UPROPERTY(BlueprintReadOnly)
		FString SkillName;

	UPROPERTY(BlueprintReadOnly)
		FString SkillInfo;

	UPROPERTY(BlueprintReadOnly)
		TArray<int> powerValues;

	UPROPERTY(BlueprintReadOnly)
		TArray<float> coolDownValues;

	UPROPERTY(BlueprintReadOnly)
		SkillSectionType SectionType = SkillSectionType::None;

	UPROPERTY(BlueprintReadOnly)
		SkillType skillType = SkillType::None;

	UPROPERTY(BlueprintReadOnly)
		int isAbility = false;

	UPROPERTY(BlueprintReadOnly)
		int point = 0;

	int powerValue = 0;
	int coolDownValue = 0;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerAbilityComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAbilityComp();

protected:
	void LoadJsonFile();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FSkillInfo* GetSkillInfo(SkillType type);
	SkillSectionType ConvertSectionType(FString type);
	SkillType ConvertSkillType(FString type);
public:
	class ATPSPlayer* myPlayer;
		

	UPROPERTY(BlueprintReadWrite)
		int skillPoint = 0;

	TArray<FSkillInfo*> skillInfos;
};
