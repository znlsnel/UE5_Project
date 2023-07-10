// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RiktorThunder.generated.h"

UCLASS()
class TPSPROJECT_API ARiktorThunder : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARiktorThunder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void SetActiveThunder(bool active, class AEnemy* parent = nullptr,  FVector GenPos = FVector(0), AActor * target = nullptr);

	void MoveLoop();
	UFUNCTION()
	void OverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FTimerHandle EndTimer;
	FTimerHandle AIMoveLoopTimer;
	bool isActiveThunder = false;

	UPROPERTY(EditAnywhere)
		class USphereComponent* playerSensor;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* mainEffect;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* AttackEffect;

	UPROPERTY(EditAnywhere)
		USoundBase* explosionSound;

	class AAIController* myAI;
	class AEnemy* myParent;
	class AActor* target;
};
