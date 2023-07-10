// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildableItem.h"
#include "TPSPlayer.h"
#include "PlayerMove.h"
#include "InventorySlot.h"
#include "BuildableItemCheckUI.h"
#include "TPSProjectGameModeBase.h"
#include "PlayerUI.h"
#include "Inventory.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/BoxComponent.h>
#include <Components/ArrowComponent.h>
#include <NavigationSystem.h>

ABuildableItem::ABuildableItem() : Super()
{
//	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SetReplicates(true);
	Tags.Add(TEXT("BuildableItem"));
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	PrimaryActorTick.bCanEverTick = true;
}

ABuildableItem::~ABuildableItem()
{

}

void ABuildableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTranceform();
}

void ABuildableItem::BeginPlay()
{
	Super::BeginPlay();
	initItem();
}


void ABuildableItem::UseItem(UInventorySlot* inventorySlot)
{

	SetActorHiddenInGame(false);

	boxCollision->SetCollisionResponseToChannel
		(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = boxCollision;

	myInventorySlot = inventorySlot;
	myPlayer->buildableItem = this;

	isSetLocation = true;
	isBuild = true;

	if (IsValid(CheckUI) == false)
	{
		CheckUI = myPlayer->BuildableItemCheckUI;
	}

	myPlayer->playerUI->ToggleInventory(false);
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
				myPlayer->playerUI->ToggleMouse(true);
				CheckUI->parentItem = this;
				CheckUI->AddToViewport();
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
	FHitResult fHit = LineTrace();
	FVector pos = fHit.ImpactPoint;

	float BoxZSize = boxCollision->GetScaledBoxExtent().Z;
	pos.Z += BoxZSize;
	//pos.Z += 200;

	SetActorLocation(pos);
}

void ABuildableItem::SetRotation()
{
	AddActorWorldRotation(FRotator(0, turnValue, 0));
}


FHitResult ABuildableItem::LineTrace(FVector StartPoint, FVector EndPoint)
{
	FVector TraceStartPoint;
	FVector LineTraceEnd;

	if (StartPoint == FVector(0) && EndPoint == FVector(0)) {
		FRotator TraceStartRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, OUT TraceStartRotation);

		float traceLength = 10000.f;
		LineTraceEnd = TraceStartPoint + TraceStartRotation.Vector() * traceLength;
	} 
	else {
		TraceStartPoint = StartPoint;
		LineTraceEnd = EndPoint;
	}


	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

	return pHitResult;
}


void ABuildableItem::UpdateTranceform()
{
	if (isBuild)
	{
		if (isSetLocation)
			SetLocation();
		else
			SetRotation();
	}
}


void ABuildableItem::completeBuilding(bool decide)
{
	if (decide)
	{
		myPlayer->buildableItem = this;
		SyncTranceform();
	}
	else
	{
		isBuild = true;
		isSetLocation = true;
	}
	myPlayer->playerUI->ToggleMouse(false);
	CheckUI->RemoveFromParent();
}


void ABuildableItem::CancelBuilding()
{
	SetActorHiddenInGame(true);
	myPlayer->buildableItem = nullptr;
	isBuild = false;
	myPlayer->playerUI->ToggleMouse(false);
}


void ABuildableItem::SyncTranceform()
{
	myPlayer->buildableItem = nullptr;
	isBuild = false;

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	boxCollision->SetCollisionResponseToChannel
	(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	if (IsValid(myInventorySlot) == false)
		myInventorySlot = myPlayer->GetInventory()->FindSameItemSlot(this);

	if (IsValid(myInventorySlot))
	{
		myInventorySlot->RemoveItemFromInventory();

		if (myInventorySlot->itemCount > 0 && myInventorySlot->Items.IsEmpty() == false) {
			Cast<ABuildableItem>(myInventorySlot->Items.Last())->UseItem(myInventorySlot);
		}
	}
	
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
}


void ABuildableItem::DamageProcess(int Damage)
{
	if (isDestroy) return;

	shield -= Damage;
	if (shield == 0)
	{
		boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DestroyActor();
	}
}