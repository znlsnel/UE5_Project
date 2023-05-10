// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoreActor.generated.h"

UCLASS()
class TPSPROJECT_API AStoreActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStoreActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	void InitializeStore(class ATPSPlayer* player);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UWidgetComponent* WidgetComp;
	class ATPSPlayer* myPlayer;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UStoreUI> storeUIFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UStoreUI* storeUI;

	UFUNCTION(Server, Reliable)
		void UpgradeAttackInServer();
		void UpgradeAttackInServer_Implementation();
		UFUNCTION(NetMulticast, Reliable)
			void UpgradeAttackInMulti();
			void UpgradeAttackInMulti_Implementation();

	UFUNCTION(Server, Reliable)
		void UpgradeHpInServer();
		void UpgradeHpInServer_Implementation();

		UFUNCTION(NetMulticast, Reliable)
			void UpgradeHpInMulti();
			void UpgradeHpInMulti_Implementation();

};
