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
	float coolDownValue = 0.f;

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
	virtual void BeginPlay()override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetMouseInput();
	void SkillTrigger();
	void SetSkillTimer(SkillType type);
	void OperateSkillTimer();
	FSkillInfo* GetSkillInfo(SkillType type);
	void SetSkillInfoArr(TArray< FSkillInfo> skillInfos);
	TArray< FSkillInfo> GetSkillInfoArr();
	class ASkill* GetSkill(SkillType type);
	bool UseSkill(SkillType type);
	bool CheckSkillPossible(SkillType type);
	bool CheckSkillPossible(ASkill* skill);
	SkillSectionType ConvertSectionType(FString type);
	SkillType ConvertSkillType(FString type);
	void UpdateValue(struct FSkillInfo* skillInfo);

public:
	class ATPSPlayer* myPlayer;
	class UScreenUI* myScreenUI;
	class ASkill* currSkill;

	FTimerHandle skillTimerHandle;

	UPROPERTY(BlueprintReadWrite)
		int skillPoint = 0;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASkill> IceAttackFactory;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASkill> LightningStrikeFactory;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASkill> HealFactory;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASkill> FireStormFactory;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* skillAnim;

	TArray<FSkillInfo*> skillInfos;
	TArray<class ASkill*> IceAttack;
	TArray<class ASkill*> LightningStrike;
	TArray<class ASkill*> HealStorm;
	TArray<class ASkill*> fireStorm;

	float LightningStrikeTime = 0.f;
	float IceAttackTime = 0.f;
	float HealTime = 0.f;
	float FireStormTime = 0.f;
};
