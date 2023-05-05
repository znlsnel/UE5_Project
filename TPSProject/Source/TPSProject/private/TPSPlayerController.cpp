// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "TPSPlayer.h"
#include "PlayerUI.h"
#include "Crosshair.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/LocalPlayer.h>
#include <Kismet/GameplayStatics.h>

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

	if (myPlayer)
	{

		myPlayer->playerUI->ATVWidgets();
	}

}
