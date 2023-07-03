// Fill out your copyright notice in the Description page of Project Settings.


#include "myGameStateBase.h"

#include <Engine/DirectionalLight.h>
#include <Kismet/GameplayStatics.h>

AmyGameStateBase::AmyGameStateBase()
{
	myLight = Cast<ADirectionalLight>( UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
}

void AmyGameStateBase::BeginPlay()
{
	DirectionalLightRotationSpeed = 360.f / (DaySecond / DirectionalLightRotateTickSpeed);
	DaytimeRotationSpeed = DirectionalLightRotationSpeed * (2 * (1 - DaytimeRatio));
	NightRotationSpeed = DirectionalLightRotationSpeed * (2 * DaytimeRatio);

	if (myLight)
		RotateDLLoop();
}

void AmyGameStateBase::RotateDLLoop()
{
	GetWorldTimerManager().ClearTimer(DLRotationTimer);

	float additionalRotationValue = DaytimeRotationSpeed;
	if (myLight->GetActorRotation().Pitch > 1)
		additionalRotationValue = NightRotationSpeed;

	myLight->AddActorWorldRotation(FRotator(additionalRotationValue, 0, 0));

	GetWorldTimerManager().SetTimer(DLRotationTimer, this, &AmyGameStateBase::RotateDLLoop, DirectionalLightRotateTickSpeed, false);
}
