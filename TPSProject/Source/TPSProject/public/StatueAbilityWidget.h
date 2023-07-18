// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatueAbilityWidget.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class AbilityType : uint8
{
	SealRepair,
	StatueRepair,
	HPUpgrade,
	None,
};

USTRUCT(BlueprintType)
struct TPSPROJECT_API FStatueAbility 
{
	GENERATED_BODY()

public:
	FString abilityName = "";
	FString abilityInfo = "";
	AbilityType abilityType = AbilityType::None;
	int point = 0;
	int cost = 0;
};

UCLASS()
class TPSPROJECT_API UStatueAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;

	void LoadJson();
	bool UpgradeAbility(AbilityType type);
	bool UpgradeAbility(FStatueAbility* ability);
	void HorverButton(bool isHorver, AbilityType type, FVector2D pos);
	void UpdateRepairRate();

	void SetGetStatueAbilityArr(TArray< FStatueAbility>& arr, float& repairRate, bool Set);

	UFUNCTION(BlueprintCallable)
		void CloseWidget();

	UFUNCTION(BlueprintImplementableEvent)
		void OpenWidget();

	AbilityType ConvertAbilityType(FString type);
	FStatueAbility* GetAbility(AbilityType type);
public:
	TArray<FStatueAbility*> statueAbilitys;

	UPROPERTY(BlueprintReadOnly)
		class ADoomstone* parent;
	UPROPERTY(BlueprintReadOnly)
		class ATPSPlayer* player;

	FTimerHandle repairRateLoopTimer; 
	FTimerHandle repairRateEndTimer; 

	UPROPERTY(BlueprintReadOnly)
		float repairRate = 0.f;

	float NextRepairRate = 0.f;
	float repairCondition = 20.f;
	bool isUpdateRepairRate = false;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UStatueAbilityInfoWidget> statueAblityInfoFactory;
	UPROPERTY(BlueprintReadOnly)
		class UStatueAbilityInfoWidget* abilityInfo;
};
