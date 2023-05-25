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
public:
	void ReturnObject(float returnTime);
	void InitArrow(FVector LookVec);
	void ShootArrow(FVector target, float power);
	void CreateDecal(class UNiagaraComponent* tempDecal, FHitResult& pHitResult);
	void CancelArrow();

public:	
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ArrowMesh;
	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComp;
	UPROPERTY(EditAnywhere)
		class UNiagaraComponent* ArrowEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Niagara)
		class UNiagaraSystem* ImpactEffect;




	bool bIsUsed = false;
	FTimerHandle returnObjectTimer;

};
