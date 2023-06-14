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
	void InitBullet(FVector pos, FRotator rot, class ATPSPlayer* player);

	UFUNCTION(BlueprintCallable)
		void HitBullet(FHitResult hit);

	void DamageProcess();
	FTimerHandle damageTimer;

	UFUNCTION()
		void BegineOverlapEvent(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlapEvent(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	bool isFire = false;
	FRotator FireRot = FRotator(0);

	UPROPERTY(EditAnywhere)
		float bulletSpeed = 2000.f;

	UPROPERTY(EditAnywhere)
		float attackPower = 300.f;

	UPROPERTY(EditAnywhere)
		float damageRange = 1500;

	UPROPERTY(EditAnywhere)
		class USphereComponent* enemySensorComp;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* explosionParticle;

	class ATPSPlayer* myPlayer;
	int enemyId = 0;
	float lastFireTime = 0.f;
};
