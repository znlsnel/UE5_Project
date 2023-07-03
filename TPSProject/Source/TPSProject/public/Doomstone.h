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
	UFUNCTION(BlueprintImplementableEvent)
		void DestoryStatue();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnHitEvent(int Damage);
	

public:
	UPROPERTY(BlueprintReadWrite)
		bool isDestory = false;

	UPROPERTY(BlueprintReadWrite)
		int Hp = 0;

	UPROPERTY(BlueprintReadWrite)
		int MaxHp = 1000;

	UPROPERTY(BlueprintReadWrite)
		float HpPercent = 1.f;

	int initHp = 1000;
};
