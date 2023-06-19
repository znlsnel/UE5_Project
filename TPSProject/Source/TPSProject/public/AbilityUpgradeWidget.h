// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerAbilityComp.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityUpgradeWidget.generated.h"

/**
 * 
 */







UCLASS()
class TPSPROJECT_API UAbilityUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAbilityUpgradeWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void UpgradeSkill(SkillType skillType, bool& result);

	UFUNCTION(BlueprintImplementableEvent)
		void OpenWidget();

	UFUNCTION(BlueprintImplementableEvent)
		void CloseWidget();

	void UpdateRemainSkillCount();

	UFUNCTION(BlueprintImplementableEvent)
		void playSkillSectionUIAnimation();
public:
	UPROPERTY(BlueprintReadWrite)
		class ATPSPlayer* myPlayer;
	class UPlayerAbilityComp* abilityComp;

	UPROPERTY(BlueprintReadWrite)
		int currSkillCoin = 10;

	int32 Unlock_SecondSection = 5;
	int32 Unlock_ThirdSection = 5;
	int32 Unlock_FourthSection = 5;

	int32 used_FirstSection = 0;
	int32 used_SecondSection = 0;
	int32 used_ThirdSection = 0;

	UPROPERTY(BlueprintReadOnly)
		int32 RemainingSecondSkillCount = 10;
	UPROPERTY(BlueprintReadOnly)
		int32 RemainingThirdSkillCount = 10;
	UPROPERTY(BlueprintReadOnly)
		int32 RemainingFourthSkillCount = 10;

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
};