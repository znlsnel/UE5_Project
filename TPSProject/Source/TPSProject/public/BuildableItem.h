// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "BuildableItem.generated.h"

/**
 * 
 */



UCLASS()
class TPSPROJECT_API ABuildableItem : public AItem
{
	GENERATED_BODY()
	
protected:
	ABuildableItem();
	~ABuildableItem();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;


public:
	void UseItem(class UInventorySlot* inventorySlot);
	void GetMouseInput(bool isPressed);
	void SetLocation();
	void SetRotation();
	FHitResult LineTrace(FVector StartPoint = FVector(0), FVector EndPoint = FVector(0));

	void UpdateTranceform();

	void completeBuilding(bool decide);
	void CancelBuilding();

	UFUNCTION(BlueprintImplementableEvent)
		void DestroyActor();

	UFUNCTION(BlueprintImplementableEvent)
		void initItem();

	UPROPERTY(BlueprintReadWrite)
		bool isDestroy = false;
	
	void SyncTranceform();

	virtual void DamageProcess(int Damage);


public:
	class UInventorySlot* myInventorySlot;
	UPROPERTY(BlueprintReadWrite)
		bool isBuild = false;

	UPROPERTY(EditAnywhere)
		BuildableItemType buildableItemType = BuildableItemType::None;

	bool isSetLocation = true;

	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class UBuildableItemCheckUI> CheckWidgetTS;

	UPROPERTY()
		class UBuildableItemCheckUI* CheckUI;

	//UPROPERTY(EditAnywhere)
		//class UArrowComponent* arrowComp;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int shield = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxShield = 100;

	float lastClickTime = 0.f;
	float turnValue = 0.f;
	static int ItemId;
};
