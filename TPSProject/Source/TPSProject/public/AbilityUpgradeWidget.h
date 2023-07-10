// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityUpgradeWidget.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct TPSPROJECT_API FUpgradeInfo
{
	GENERATED_BODY()
public:
	int32 Unlock_SecondSection = 5;
	int32 Unlock_ThirdSection = 5;
	int32 Unlock_FourthSection = 5;

	int32 used_FirstSection = 0;
	int32 used_SecondSection = 0;
	int32 used_ThirdSection = 0;

	int32 RemainingSecondSkillCount = 5;
	int32 RemainingThirdSkillCount = 10;
	int32 RemainingFourthSkillCount = 15;

	float toSecondSectionRate = 0.f;
	float toThirdSectionRate = 0.f;
	float toFourthSectionRate = 0.f;
};


UCLASS()
class TPSPROJECT_API UAbilityUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct()override;
	UFUNCTION(BlueprintImplementableEvent)
		void InitWidget();


	UFUNCTION(BlueprintCallable)
		void UpgradeSkill(SkillType skillType, bool& result);

	UFUNCTION(BlueprintCallable)
		void SkillInfoWidgetEvent(bool isHover, SkillType type, FVector2D pos);

	UFUNCTION(BlueprintImplementableEvent)
		void OpenWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void CloseWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void playSkillSectionUIAnimation();

	void UpdateRemainSkillCount();
	FUpgradeInfo GetAbilityInfo();
	void SetAbilityInfo(FUpgradeInfo upgradeInfo);

public:
	class ATPSPlayer* myPlayer;
	class UPlayerAbilityComp* abilityComp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UAbilityInfo> AbilityInfoFactory;

	UPROPERTY(BlueprintReadOnly)
		class UAbilityInfo* abliltyInfo;

	UPROPERTY(BlueprintReadOnly)
		int currSkillCoin = 1;

	int32 Unlock_SecondSection = 5;
	int32 Unlock_ThirdSection = 5;
	int32 Unlock_FourthSection = 5;

	int32 used_FirstSection = 0;
	int32 used_SecondSection = 0;
	int32 used_ThirdSection = 0;

	UPROPERTY(BlueprintReadOnly)
		int32 RemainingSecondSkillCount = 5;
	UPROPERTY(BlueprintReadOnly)
		int32 RemainingThirdSkillCount = 10;
	UPROPERTY(BlueprintReadOnly)
		int32 RemainingFourthSkillCount = 15;

	UPROPERTY(BlueprintReadOnly)
		float toSecondSectionRate = 0.f;
	UPROPERTY(BlueprintReadOnly)
		float toThirdSectionRate = 0.f;
	UPROPERTY(BlueprintReadOnly)
		float toFourthSectionRate = 0.f;

	UPROPERTY(EditAnywhere)
		USoundBase* successSound;
	UPROPERTY(EditAnywhere)
		USoundBase* failedSound;

	UPROPERTY(BlueprintReadWrite)
		bool isOpen = false;

};