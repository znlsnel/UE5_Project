// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PickupManager.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPickupManager : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	template<bool b>
	void PickupObject() { PickupObject(b); }
	void PickupObject(bool isPressed);
	FHitResult LineTrace();

	UFUNCTION(BlueprintCallable)
		void CompletedProgressBar();

public:
	// PrograssBar
	UPROPERTY(EditDeFaultsOnly, Category = ProgressBar)
		TSubclassOf<class UPickUpPB> progressBarUI;

	class UPickUpPB* _progressBarUI;

	class AItem* pickupItem;

	UPROPERTY(EditAnywhere)
		int pickupRange = 300;

};
