// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "TPSPlayer.h"
#include "PlayerUI.h"
#include "Crosshair.h"
#include "StoreActor.h"
#include "StoreUI.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/LocalPlayer.h>
#include <Kismet/GameplayStatics.h>
#include <Components/WidgetComponent.h>

ATPSPlayerController::ATPSPlayerController()
{

	//FString OutErr = FString("omg..");
	//// Create a new LocalPlayer object

	//int id = id_counter;
	//ULocalPlayer* localPlayer = GetGameInstance()->CreateLocalPlayer(id, OutErr, true);

	//localPlayer->SetControllerId(id);
	//id_counter++;
	//// Add the LocalPlayer to the game instance
	//GetGameInstance()->AddLocalPlayer(localPlayer, localPlayer->GetControllerId());

	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("LocalPlayer created and added to game instance"));

}

void ATPSPlayerController::BeginPlay()
{
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//GetWorldTimerManager().SetTimer(atvTimer, this, &ATPSPlayerController::ATVWidget, 1.5f);

}

void ATPSPlayerController::ATVWidget()
{
	if (myPlayer)
	{
		myPlayer->playerUI->ATVWidgets();
		myPlayer->StartGame();
	}



	//storeActor = GetWorld()->SpawnActor<AStoreActor>(storeActorFactory);
	//storeActor->InitializeStore(myPlayer);
	//storeActor->SetActorHiddenInGame(false);
	//storeActor->WidgetComp->GetWidget()->SetOwningPlayer(this);

}
