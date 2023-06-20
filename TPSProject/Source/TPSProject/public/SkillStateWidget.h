// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PlayerAbilityComp.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API USkillStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

public:
	UPROPERTY(BlueprintReadWrite)
		class UAbilityUpgradeWidget* parent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		SkillType skillType = SkillType::None;
	UPROPERTY(EditAnywhere)
		SkillSectionType skillSectionType = SkillSectionType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* skillTexture;

};
