// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemFactoryComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UItemFactoryComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemFactoryComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABuildableItem> StoneWall;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABuildableItem> Barricade;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABuildableItem> BrokenWalll;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABuildableItem> Sandbag;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABuildableItem> Turret;
		
};
