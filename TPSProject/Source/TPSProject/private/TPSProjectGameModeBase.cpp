// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSProjectGameModeBase.h"
#include "TPSPlayer.h"
#include "EnemyManager.h"
#include "TPSPlayer.h"

#include <GameFramework/GameState.h>
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include<Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>


#include <Net/UnrealNetwork.h>

ATPSProjectGameModeBase::ATPSProjectGameModeBase()
{
	SetReplicates(true);
	GetWorld()->Exec(GetWorld(), TEXT("DisableAllScreenMessages"));

}

void ATPSProjectGameModeBase::InitGame(const FString& mapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(mapName, Options, ErrorMessage);

	// Create and load a LocalPlayer

}

void ATPSProjectGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSProjectGameModeBase, EnemyManager);
	//DOREPLIFETIME(ATPSProjectGameModeBase, managedActors);
}

void ATPSProjectGameModeBase::BeginPlay()
{
	GetWorld()->Exec(GetWorld(), TEXT("DisableAllScreenMessages"));
	Super::BeginPlay();

	//GetWorldTimerManager().SetTimer(StartTimer, this, &ATPSProjectGameModeBase::StartGameInServer, 15.5f);
}

void ATPSProjectGameModeBase::RegisterActor(AActor* actorTpRegister)
{
	managedActors.Add(actorTpRegister);
}

void ATPSProjectGameModeBase::RegisterActorServer_Implementation()
{
	RegisterActorMulti();
}

void ATPSProjectGameModeBase::RegisterActorMulti_Implementation()
{
	if (tempActor)
		managedActors.Add(tempActor);
}


void ATPSProjectGameModeBase::StartGameInServer_Implementation()
{
	StartGame();
}

void ATPSProjectGameModeBase::StartGame_Implementation()
{
	EnemyManager = Cast<AEnemyManager>(GetWorld()->SpawnActor(enemyManagerFactory));
}


