// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Doomstone.generated.h"

UCLASS()
class TPSPROJECT_API ADoomstone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoomstone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnHitEvent(int Damage);
	
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStatueHp();



public:
	UPROPERTY(BlueprintReadWrite)
		bool isDestory = false;





	UPROPERTY(BlueprintReadWrite)
		int Hp = 0;

	UPROPERTY(BlueprintReadWrite)
		int MaxHp = 100;


};
