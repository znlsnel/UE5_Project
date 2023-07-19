// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class TPSPROJECT_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	class AEnemy* tempEnemy;
public:
	void ReturnObject(float returnTime);
	void InitArrow(FVector LookVec, int damage);
	bool ShootArrow(FVector target, float power);
	void CreateDecal(class UNiagaraComponent* tempDecal, FHitResult& pHitResult);
	void CancelArrow();

public:	
	class ATPSPlayer* myPlayer;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ArrowMesh;
	UPROPERTY(EditAnywhere)
		class UPointLightComponent* Light;
	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComp;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* ArrowEffect;

	UPROPERTY(EditDefaultsOnly)
		class UNiagaraSystem* ImpactEffect;

	int attackDamage = 50;
	int addDamage = 0;
	int doubleAttackRate = 0;

	bool bIsUsed = false;
	FTimerHandle returnObjectTimer;
	FTimerHandle doubleAttackTimer;

};
