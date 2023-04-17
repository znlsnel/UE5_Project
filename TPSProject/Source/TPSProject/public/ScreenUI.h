// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenUI.generated.h"



/**
 * 
 */
UCLASS()
class TPSPROJECT_API UScreenUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateScreenUI();
	void Initialization(class ATPSPlayer* player);

public:
	class ATPSPlayer* myPlayer;


	UPROPERTY(EditAnywhere, blueprintReadWrite, category = Player)
		float hpRatio;
	UPROPERTY(EditAnywhere, blueprintReadWrite, category = Player)
		FString hpInfo;
	
	UPROPERTY(EditAnywhere)
		UTexture* PistolUI;
	UPROPERTY(EditAnywhere)
		UTexture* RifleUI;
	UPROPERTY(EditAnywhere)
		UTexture* ShotgunUI;

	UPROPERTY(BlueprintReadWrite)
		FWeaponInfo primaryInfo;

	UPROPERTY(BlueprintReadWrite)
		FWeaponInfo secondaryInfo;
	

	// ÇÔ¼ö ##############################################

	UFUNCTION(BlueprintNativeEvent)
		void WeaponSwap();

};
