// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "myGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AmyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AmyGameStateBase();
	virtual void BeginPlay()override;
	
	void RotateDLLoop();
public:
	FTimerHandle DLRotationTimer;

	UPROPERTY(EditDefaultsOnly)
		float DaySecond = 180.f;

	UPROPERTY(EditDefaultsOnly)
		float DirectionalLightRotateTickSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly)
		float DaytimeRatio = 0.35f;

	float DirectionalLightRotationSpeed = 0.f;
	float DaytimeRotationSpeed = 0.f;
	float NightRotationSpeed = 0.f;

	class ADirectionalLight* myLight;
	class ADoomstone* statue;
};
