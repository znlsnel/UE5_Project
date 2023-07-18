// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TPSPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddWorldDamageUI(int Damage,float currHpPercent, float preHpPercent);
	void OnDamage(int damage, FName boneName ="", class ATPSPlayer* AttackPlayer = nullptr, bool ignoreHpEvent = false );
	bool isActive();
	bool isDead();
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	bool isOverlapingTargets();
	void OnAttackTargets(TArray<AActor*> actors);
	void StopMove();
public:
	FTimerHandle DeathDelayTimer;
	FTimerHandle damageWidgetInitTimer;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* targetSensor;

	UPROPERTY(VisibleAnywhere)
		class UEnemyFSM* fsm;

	class AController* myController;
	class AEnemyManager* enemyManager;
	class ATPSPlayer* player;



	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemyHpBar> hpBarFactory;
	class UEnemyHpBar* hpBar;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* damageEffect;

	UPROPERTY()
		class UDamageWidget* damageWidget;
};
