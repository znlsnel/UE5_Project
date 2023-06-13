// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildableItem.h"
#include "TurretItem.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ATurretItem : public ABuildableItem
{
	GENERATED_BODY()
protected:
	ATurretItem();

public:
	virtual void Tick(float DeltaSecond)override;

	void FireLoop();
	class AEnemy* FindEnemy();
	bool isShootingPossible(class AEnemy* enemy);
	UFUNCTION(Blueprintpure)
		bool isTargetsEmpty();

	virtual void BeginPlay()override;
	
	UFUNCTION(BlueprintImplementableEvent)
		void GetFirePos();

	class ATurretBullet* GetBullet();
public:
	UPROPERTY(BlueprintReadWrite)
		FVector muzzlePos;

	UPROPERTY(BlueprintReadWrite)
		class AEnemy* currTarget;

	int targetId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USphereComponent* enemySensorComp;
	float sensorRange = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float fireSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Damage = 10.f;

	UPROPERTY(EditAnywhere)
		TSubclassOf <class ATurretBullet> bulletFactory;
	TArray<class ATurretBullet*> bullets;
	int bulletCount = 10;

	FTimerHandle fireLoopTimer;

};
