// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StatueAbilityWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatueAbilityWidgetButton.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UStatueAbilityWidgetButton : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct()override;

	UFUNCTION(BlueprintImplementableEvent)
		void InitWidget();

	UFUNCTION(BlueprintCallable)
		void UpgradeAbility(bool& result);

	UFUNCTION(BlueprintCallable)
		void HorverButton(bool isHorver, FVector2D pos);

public:
	class ATPSPlayer* myPlayer;
	class UStatueAbilityWidget* myAbilityWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* AbilityTexture;

	UPROPERTY(EditAnywhere)
		AbilityType myAbilityType = AbilityType::None;
};
