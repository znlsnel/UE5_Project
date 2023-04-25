// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "TPSPlayer.h"

#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
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
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("DropItem"));
	//DetachRootComponentFromParent();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FVector tempPos = myPlayer->GetActorLocation();
	FRotator tempRot = myPlayer->GetActorRotation();
	tempPos += myPlayer->GetActorForwardVector() * 50;
	tempPos.Z = 232 - 149;
	tempRot.Pitch = 0;

	SetActorLocation(tempPos);
	SetActorRotation(tempRot);
	SetActorHiddenInGame(false);

}

void AItem::CreatePickupCollision()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("CreateCollision"));
	if (pickupCollision)
		pickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AItem::RemovePickupCollision()
{
	if (pickupCollision)
		pickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
}
