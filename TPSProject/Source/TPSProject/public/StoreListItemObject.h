// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStoreUI.h"

#include "UObject/NoExportTypes.h"
#include "StoreListItemObject.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UStoreListItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	FStoreItem itemIfno;
	UItemStoreUI* parentStore;
};
