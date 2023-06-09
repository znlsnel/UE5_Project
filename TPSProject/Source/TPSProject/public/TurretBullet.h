// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretBullet.generated.h"

UCLASS()
class TPSPROJECT_API ATurretBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FireBullet(float DeltaTime);
	void InitBullet(FVector pos, FRotator rot);

	UFUNCTION(BlueprintCallable)
		void HitBullet(AActor* HitTarget);

	void DamageProcess(class AEnemy* target);

public:
	bool isFire = false;

	UPROPERTY(EditAnywhere)
		float bulletSpeed = 1000.f;

	UPROPERTY(EditAnywhere)
		float attackPower = 30.f;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* explosionParticle;
};
