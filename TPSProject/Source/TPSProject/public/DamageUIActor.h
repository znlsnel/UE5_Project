// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageUIActor.generated.h"

UCLASS()
class TPSPROJECT_API ADamageUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageUIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddWorldDamage(float DeltaTime);
	void InitializeDamageActor(FVector startPos, FRotator rotator, int Damage, class ATPSPlayer* p);

	UFUNCTION(Server, Reliable)
		void SetRot();
		void SetRot_Implementation();

		UFUNCTION(Client, Reliable)
			void SetRot_M();
			void SetRot_M_Implementation();
public:
	UPROPERTY(VisibleAnywhere, category = DamageUI)
		class UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere, category = DamageUI)
		TSubclassOf<class UDamageUI> damageUIFactory;
	class UDamageUI* damageUI;

	bool Trigger = false;
	float addedZPos = 0.f;
	class ATPSPlayer* myPlayer;

};
