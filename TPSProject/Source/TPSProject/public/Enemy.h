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

	void AddWorldDamageUI(FRotator genRot, int Damage);
	void DieEvent();

public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UEnemyFSM* fsm;

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class ADamageUIActor> damageActorFactory;

	TArray<class ADamageUIActor*> damageActorArr;
	int32 DActorCount = 5;
	int32 DActorindex = 0;

	class AEnemyManager* enemyManager;
};
