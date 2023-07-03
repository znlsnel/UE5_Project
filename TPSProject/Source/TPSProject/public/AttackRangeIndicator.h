// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackRangeIndicator.generated.h"

UCLASS()
class TPSPROJECT_API AAttackRangeIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackRangeIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetActive(bool active, FVector pos = FVector(0), float setTime = 3.f);
	void SetAttackTimer(class AEnemy* enemy, float setTime = 1.5f);
	void SetMode(bool isMeleeMode);
	void AttackToTargets();

public:
	FTimerHandle AttackTimer;
	FTimerHandle EndTimer;

	UPROPERTY(EditAnywhere)
		class USphereComponent* targetSensor;

	UPROPERTY(EditAnywhere)
		class UDecalComponent* decal;

	UPROPERTY(EditAnywhere)
		class UPointLightComponent* lightComponent;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* SkillEffect;

	UPROPERTY(EditAnywhere)
		class USoundBase* explosionSound;

	bool isActiveDecal = false;

	class AEnemy* myEnemy;

};
