// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildableItem.h"
#include "BarricadeItem.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ABarricadeItem : public ABuildableItem
{
	GENERATED_BODY()
	
public:
	ABarricadeItem();
	virtual void BeginPlay()override;
	

public:

};
