// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSPlayerController();
	//static int id_counter;
	virtual void BeginPlay()override;
	void ATVWidget();
	class ATPSPlayer* myPlayer;

	FTimerHandle atvTimer;

	UPROPERTY(EditAnywhere, Category = Store)
		TSubclassOf<class AStoreActor> storeActorFactory;

	UPROPERTY(blueprintReadWrite)
		class AStoreActor* storeActor;

};
