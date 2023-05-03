// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSProjectGameModeBase.h"
#include "EnemyManager.h"
#include "TPSPlayer.h"

#include <GameFramework/GameState.h>
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include<Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>

ATPSProjectGameModeBase::ATPSProjectGameModeBase()
{

	



}

void ATPSProjectGameModeBase::InitGame(const FString& mapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(mapName, Options, ErrorMessage);

	// Create and load a LocalPlayer

}

void ATPSProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Hello Im GameModeBase"));
	UGameInstance* tempGameInstance = GetGameInstance();

	players = tempGameInstance->GetLocalPlayers();
	int32 playerNum = tempGameInstance->GetNumLocalPlayers();

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("playerNum :  %d"),playerNum));

	for (ULocalPlayer* player : players)
	{
		testWidget = CreateWidget(player->GetPlayerController(GetWorld()), widgetFactory);
		testWidget->AddToViewport();
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("AddToViewPort"));
	}


}


void ATPSProjectGameModeBase::StartGame()
{

}
