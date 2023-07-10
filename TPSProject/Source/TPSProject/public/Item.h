// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class TPSPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void DropItemOnGround();
	void CreatePickupCollision();
	void RemovePickupCollision();

	UFUNCTION(client, Reliable)
		void DropItemInServer();
		void DropItemInServer_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void DropItemMulticast();
		void DropItemMulticast_Implementation();



public:
	class ATPSPlayer* myPlayer;
	UPROPERTY(EditAnywhere)
		ItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponTypes)
		UTexture2D* ItemIcon;

	UPROPERTY(EditAnyWhere)
		class UBoxComponent* pickupCollision;
};
