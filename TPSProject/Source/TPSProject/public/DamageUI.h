// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UDamageUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void BindDamageText(int damage);
};
