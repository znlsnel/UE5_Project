// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BomberMan.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ABomberMan : public AEnemy
{
	GENERATED_BODY()
	
public:
	ABomberMan();
	virtual void BeginPlay()override;

	UFUNCTION()
		void TriggerBomber(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Explosion();
public:
	UPROPERTY(EditAnywhere)
		class USoundBase* explosionSound;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* explosionEffect;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* Sensor;

};
