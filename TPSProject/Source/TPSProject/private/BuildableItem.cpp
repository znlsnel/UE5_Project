// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildableItem.h"
#include "TPSPlayer.h"
#include "PlayerMove.h"
#include "InventorySlot.h"
#include "BuildableItemCheckUI.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/BoxComponent.h>

ABuildableItem::ABuildableItem()
{
	Tags.Add(TEXT("BuildableItem"));
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	PrimaryActorTick.bCanEverTick = true;

}

void ABuildableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isBuild)
	{

		if (isSetLocation)
			SetLocation();
		else
			SetRotation();
	}
}

void ABuildableItem::BeginPlay()
{
	Super::BeginPlay();



}

#include "PlayerUI.h"
void ABuildableItem::UseItem(UInventorySlot* inventorySlot)
{

	SetActorHiddenInGame(false);

	boxCollision->SetCollisionResponseToChannel
		(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	//boxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
	//	ECollisionResponse::ECR_Ignore);

	myInventorySlot = inventorySlot;
	isSetLocation = true;
	myPlayer->buildableItem = this;
	isBuild = true;

	if (IsValid(CheckUI) == false)
	{
		CheckUI = CreateWidget<UBuildableItemCheckUI>(GetWorld(), CheckWidgetTS);
		CheckUI->parentItem = this;
	}

	myPlayer->playerUI->ToggleInventory();
	myPlayer->playerUI->ToggleMouse(false);
}
void ABuildableItem::GetMouseInput(bool isPressed)
{
	if (isPressed)
	{
		isSetLocation = false;
		lastClickTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() -lastClickTime < 0.2) {
			if (IsValid(CheckUI)) {
				CheckUI->OpenCheckUI();
				isBuild = false;
			}

		}
		else
		{
			isBuild = true;
			isSetLocation = true;
		}
	}
}

void ABuildableItem::SetLocation()
{
//	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SetLocation"));
	FHitResult fHit = LineTrace();
	FVector pos = fHit.ImpactPoint;

	SetActorLocation(pos);
}

void ABuildableItem::SetRotation()
{


	AddActorWorldRotation(FRotator(0, turnValue, 0));
}


FHitResult ABuildableItem::LineTrace()
{
	FVector TraceStartPoint;
	FRotator TraceStartRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, OUT TraceStartRotation);

	float traceLength = 10000.f;
	FVector LineTraceEnd = TraceStartPoint + TraceStartRotation.Vector() * traceLength;

	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

	return pHitResult;
}

void ABuildableItem::completeBuilding(bool decide)
{
	if (decide)
	{
		myPlayer->buildableItem = nullptr;

		boxCollision->SetCollisionResponseToChannel
		(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

		//boxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
		//	ECollisionResponse::ECR_Ignore);
		myInventorySlot->RemoveItemFromInventory();

		myPlayer->buildableItem = this;
		isBuild = false;
	}
	else
	{
		isBuild = true;
		isSetLocation = true;
	}
	myPlayer->playerUI->ToggleMouse(false);

}

void ABuildableItem::CancelBuilding()
{
	SetActorHiddenInGame(true);
	isBuild = false;
	myPlayer->playerUI->ToggleMouse(false);

}

