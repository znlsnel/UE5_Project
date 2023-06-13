// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildableItem.h"
#include "BarricadeItem.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ABarricadeItem : public ABuildableItem
{
	GENERATED_BODY()
	
public:
	ABarricadeItem();
	virtual void BeginPlay()override;
	

public:
	UFUNCTION()
		void BegineOverlapEvent(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void EndOverlapEvent(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void DamageProcess(int Damage)override;


public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* overlapCollision;

	TMap<class AEnemy*,  INT> enemys;
	int enemyId = 0;
};
