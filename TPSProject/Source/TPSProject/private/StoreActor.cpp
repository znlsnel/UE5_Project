// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreActor.h"
#include "StoreUI.h"
#include "TPSPlayer.h"
#include <Components/WidgetComponent.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AStoreActor::AStoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	//WidgetComp->SetIsReplicated(true);
	//WidgetComp->SetNetAddressable();


}

// Called when the game starts or when spawned
void AStoreActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(635, 40, 175));
	SetActorRotation(FRotator(0, -180, 0));
}

void AStoreActor::InitializeStore(ATPSPlayer* player)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Create Store!!!"));
	myPlayer = player;

	storeUI = CreateWidget<UStoreUI>(GetWorld(), storeUIFactory);
	storeUI->storeActor = this;
	
	//WidgetComp->SetRelativeRotation(FRotator(0, -180, 0));
	WidgetComp->SetWorldRotation(FRotator(0, -180, 0));

	
	WidgetComp->SetWidget(storeUI);
	//storeUI->SetVisibility(ESlateVisibility::Hidden);
	SetActorHiddenInGame(true);
	//Cast<UStoreUI>(WidgetComp->GetWidget())->storeActor = this;
}


// Called every frame
void AStoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStoreActor::UpgradeAttackInServer_Implementation()
{
	UpgradeAttackInMulti();
}

void AStoreActor::UpgradeAttackInMulti_Implementation()
{
	storeUI->UpgradeAttack();
}

void AStoreActor::UpgradeHpInServer_Implementation()
{
	UpgradeHpInMulti();
}

void AStoreActor::UpgradeHpInMulti_Implementation()
{
	storeUI->UpgradeHp();
}
