// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildableItem.h"
#include "TPSPlayer.h"
#include "PlayerMove.h"
#include "InventorySlot.h"
#include "BuildableItemCheckUI.h"
#include "TPSProjectGameModeBase.h"
#include "PlayerUI.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/BoxComponent.h>
#include <Net/UnrealNetwork.h>
#include <Components/ArrowComponent.h>

ABuildableItem::ABuildableItem()
{
//	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SetReplicates(true);
	Tags.Add(TEXT("BuildableItem"));
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	PrimaryActorTick.bCanEverTick = true;
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

void ABuildableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ABuildableItem::UseItem(UInventorySlot* inventorySlot)
{

	SetActorHiddenInGame(false);

	boxCollision->SetCollisionResponseToChannel
		(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//boxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
	//	ECollisionResponse::ECR_Ignore);
	RootComponent = boxCollision;
	myInventorySlot = inventorySlot;
	isSetLocation = true;
	myPlayer->buildableItem = this;
	isBuild = true;

	if (IsValid(CheckUI) == false)
	{
		//CheckUI = CreateWidget<UBuildableItemCheckUI>(GetWorld(), CheckWidgetTS);
		CheckUI = CreateWidget<UBuildableItemCheckUI>(GetWorld(), CheckUIFactory);
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
		//boxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
		//	ECollisionResponse::ECR_Ignore);
		myPlayer->buildableItem = this;
		myPlayer->SetTranceformBuildableItem(this, GetActorLocation(), GetActorRotation());

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
	myPlayer->buildableItem = nullptr;
	isBuild = false;
	myPlayer->playerUI->ToggleMouse(false);

}
//
//void ABuildableItem::SyncTranceform_Implementation()
//{
//	SyncTranceformMulti();
//}


void ABuildableItem::SyncTranceform(FVector lot, FRotator rot)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("loc : %f, %f, %f \n rot : %f, %f, %f"), lot.X, lot.Y, lot.Z, rot.Roll, rot.Yaw, rot.Pitch));

	myPlayer->buildableItem = nullptr;
	isBuild = false;

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	boxCollision->SetCollisionResponseToChannel
	(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	isBuild = false;

	SetActorLocation(lot);
	SetActorRotation(rot);

	if (IsValid(myInventorySlot)) {
		myInventorySlot->RemoveItemFromInventory();
		if (myInventorySlot->itemCount > 0 && myInventorySlot->Items.IsEmpty() == false) {
			Cast<ABuildableItem>(myInventorySlot->Items.Last())->UseItem(myInventorySlot);
		}
	}

}

void ABuildableItem::DamageProcess()
{
	if (isDestroy) return;
	shield--;
	if (shield == 0)
	{
		
	}
}

