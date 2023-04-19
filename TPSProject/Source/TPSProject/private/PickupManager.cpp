// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManager.h"
#include "Weapon.h"
#include "Weapon_Pistol.h"
#include "Weapon_Rifle.h"
#include "Weapon_Shotgun.h"
#include "WeaponData.h"
#include "PickUpPB.h"

#include <Kismet/KismetSystemLibrary.h>


void UPickupManager::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Pressed, this, &UPickupManager::PickupObject<true>);
	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Released, this, &UPickupManager::PickupObject<false>);

	_progressBarUI = Cast<UPickUpPB>(CreateWidget(GetWorld(), progressBarUI));
	_progressBarUI->pickupManager = this;
}

void UPickupManager::PickupObject(bool isPressed)
{
	if (isPressed)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Press!")));

		FHitResult hitResult = LineTrace();

		tempWeapon = hitResult.GetActor();
		if (hitResult.bBlockingHit && tempWeapon && tempWeapon->ActorHasTag(TEXT("Weapon")))
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Weapon!")));
			// 프로그래스바
			_progressBarUI->AddToViewport();
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("NO!")));
	
		}
		
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Release")));
		if (_progressBarUI->IsInViewport())
			_progressBarUI->RemoveFromParent();
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
	IgnoreActor.Add(me);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), startPoint, endPoint, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, IgnoreActor, EDrawDebugTrace::None, hitResult, true);

	return hitResult;
}

void UPickupManager::CompletedProgressBar()
{
	Cast<AWeapon>(tempWeapon)->SynchronizeWhitPlayer(me);
}
