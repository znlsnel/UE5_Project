// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "TPSPlayer.h"
#include "PlayerUI.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>


#include <Net/UnrealNetwork.h>
// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::BindPlayer(ATPSPlayer* player)
{
	myPlayer = player;
	ToggleWidget(true);
	player->InteractionDelegate.BindUObject(this, &AInteractableActor::InteractiionStart);
}


void AInteractableActor::UnBindPlayer(ATPSPlayer* player)
{
	ToggleWidget(false);


	player->InteractionDelegate.Unbind();
	player->playerUI->ToggleMouse(false);

}

void AInteractableActor::InteractiionStart()
{
	ToggleWidget(false);

	myPlayer->playerUI->ToggleMouse(true);


	Interaction();
}

