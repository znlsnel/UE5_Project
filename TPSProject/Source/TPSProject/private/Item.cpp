// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "TPSPlayer.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Item"));
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::DropItemOnGround()
{
	//DetachRootComponentFromParent();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FVector tempPos = myPlayer->GetActorLocation();
	FRotator tempRot = myPlayer->GetActorRotation();
	tempPos += myPlayer->GetActorForwardVector() * 50;
	tempPos.Z = 0;
	tempRot.Pitch = 0;

	SetActorLocation(tempPos);
	SetActorRotation(tempRot);
}

