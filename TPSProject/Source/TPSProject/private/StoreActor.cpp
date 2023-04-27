// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreActor.h"
#include "StoreUI.h"
#include <Components/WidgetComponent.h>

// Sets default values
AStoreActor::AStoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
}

// Called when the game starts or when spawned
void AStoreActor::BeginPlay()
{
	Super::BeginPlay();
	WidgetComp->SetWidget(storeUI);
	SetActorLocation(FVector(635, 40, 175));
	SetActorRotation(FRotator(0, -180, 0));
}

// Called every frame
void AStoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

