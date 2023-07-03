// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueAbilityWidgetButton.h"
#include "TPSPlayer.h"
#include "PlayerUI.h"

#include <Kismet/GameplayStatics.h>

void UStatueAbilityWidgetButton::NativeConstruct()
{
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	myAbilityWidget = myPlayer->playerUI->statueAbilityWidget;

	InitWidget();
}

void UStatueAbilityWidgetButton::UpgradeAbility(bool& result)
{

	myAbilityWidget->UpgradeAbility(myAbilityType);
}

void UStatueAbilityWidgetButton::HorverButton(bool isHorver, FVector2D pos)
{
	myAbilityWidget->HorverButton(isHorver, myAbilityType, pos);
}
