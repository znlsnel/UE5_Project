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
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

public:
	void UseItem(class UInventorySlot* inventorySlot);
	void GetMouseInput(bool isPressed);
	void SetLocation();
	void SetRotation();
	FHitResult LineTrace();

	UFUNCTION(BlueprintCallable)
		void completeBuilding(bool decide);
	UFUNCTION(BlueprintCallable)
		void CancelBuilding();

public:
	class UInventorySlot* myInventorySlot;
	UPROPERTY(BlueprintReadWrite)
		bool isBuild = false;

	bool isSetLocation = true;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UBuildableItemCheckUI> CheckWidgetTS;
	class UBuildableItemCheckUI* CheckUI;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxCollision;

	float lastClickTime = 0.f;
	float turnValue = 0.f;
};
