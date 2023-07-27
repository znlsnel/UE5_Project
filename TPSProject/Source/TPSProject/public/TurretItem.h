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
	virtual void BeginPlay()override;

	void FireLoop();
	void FindEnemy();
	bool isShootingPossible(class AEnemy* enemy, FHitResult& hitResult);
	UFUNCTION(Blueprintpure)
		bool isTargetsEmpty();
	void CreateEffect(FHitResult& hit);

public:
	FTimerHandle findEnemyTimer;

	UPROPERTY(BlueprintReadWrite)
		FVector muzzlePos;



	int targetId = 0;
	
	UPROPERTY(EditDefaultsOnly)
		float turretRotationSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USphereComponent* enemySensorComp;
	float sensorRange = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float fireSpeed = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Damage = 5.f;


	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* myCylinder;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* myMuzzle;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* fireEffect;
	UPROPERTY(EditDefaultsOnly)
		class UNiagaraSystem* bulletTracer;
	UPROPERTY(EditDefaultsOnly)
		class UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere)
		class USoundBase* fireSound;

	int bulletCount = 10;

	FTimerHandle fireLoopTimer;
	
	UPROPERTY(BlueprintReadWrite)
		class AEnemy* currTarget;

	bool isTurretActive = false; 
	bool isFireable = false;
};
