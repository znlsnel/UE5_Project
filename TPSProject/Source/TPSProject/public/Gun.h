// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSPlayer.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"


UCLASS()
class TPSPROJECT_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void AcquireWeapon();


public:
	void PickUpCheck();

public:
	bool bIsOverlap = false;
	ATPSPlayer* player = nullptr;
	UUserWidget* pickUpWD = nullptr;

public:
	UPROPERTY(VisibleAnywhere, category = BodyMesh)
		class USkeletalMeshComponent* bodyMeshComp;

	UPROPERTY(VisibleAnywhere, category = Collision)
		class USphereComponent* acquireCollision;

	UPROPERTY(EditDefaultsOnly, category = UserWidget)
		TSubclassOf<class UUserWidget> Ts_PickUpWD;
	

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

};
