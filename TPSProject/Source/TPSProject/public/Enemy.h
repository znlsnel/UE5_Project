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

	void AddWorldDamageUI(int Damage);
	
	void DieEvent(class ATPSPlayer* AttackPlayer);
	void OnDamage(int damage, FName boneName ="", class ATPSPlayer* AttackPlayer = nullptr);
	void SetTarget(AActor* target);
	bool isActive();
public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UEnemyFSM* fsm;

	class AController* myController;
	class AEnemyManager* enemyManager;
	class ATPSPlayer* player;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* HpBarWgComp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemyHpBar> HpBarWg;
	class UEnemyHpBar* HpBar;
};
