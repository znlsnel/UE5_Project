// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManager.h"
#include "Weapon.h"
#include "Weapon_Pistol.h"
#include "Weapon_Rifle.h"
#include "Weapon_Shotgun.h"
#include "WeaponData.h"
#include "PickUpPB.h"
#include "PlayerUI.h"
#include "ScreenUI.h"
#include "Inventory.h"
#include "Item.h"

#include <Kismet/KismetSystemLibrary.h>


void UPickupManager::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Pressed, this, &UPickupManager::PickupObject<true>);
	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Released, this, &UPickupManager::PickupObject<false>);

	_progressBarUI = Cast<UPickUpPB>(CreateWidget(GetWorld(), progressBarUI));
	_progressBarUI->pickupManager = this;
	_progressBarUI->AddToViewport();
}

void UPickupManager::PickupObject(bool isPressed)
{
	if (isPressed)
	{ 
		FHitResult hitResult = LineTrace();

		float length = FVector::Dist(me->GetActorLocation(), hitResult.ImpactPoint);

		if (length > pickupRange) return;
		
		pickupItem = Cast<AItem>(hitResult.GetActor());
		if (pickupItem && pickupItem->ActorHasTag(TEXT("Item")))
		{
			_progressBarUI->PlayProgressBar();
		}
	}

	else
	{
		if (_progressBarUI->IsInViewport())
		{
			_progressBarUI->StopProgressBar();
			//_progressBarUI->RemoveFromViewport();
		}
	}
}

FHitResult UPickupManager::LineTrace()
{
	FVector startPoint;
	FVector endPoint;
	FRotator viewRotator;

	me->GetController()->GetPlayerViewPoint(OUT startPoint, OUT viewRotator);

	endPoint = startPoint + (viewRotator.Vector() * 10000);

	FHitResult hitResult;
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(GetOwner());

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), startPoint, endPoint, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, IgnoreActor, EDrawDebugTrace::None, hitResult, true);

	return hitResult;
}

void UPickupManager::CompletedProgressBar()
{
	if (pickupItem == nullptr) return;
	
	pickupItem->myPlayer = me;

	me->GetInventory()->AddItemToInventory(pickupItem);
	//Cast<AWeapon>(pickupItem)->SynchronizeWhitPlayer(me);
}
