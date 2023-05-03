// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"
#include "TPSPlayer.h"
#include <Kismet/KismetSystemLibrary.h>

#include <Net/UnrealNetwork.h>
void UCrosshair::Initialization(ATPSPlayer* player)
{
	
	myPlayer = player;

	if (myPlayer->GetLocalRole() == ROLE_Authority)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Im Server"));
		return;
	}
	


	if (GetWorld()->GetFirstPlayerController())
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Im Client"));
	}
}

void UCrosshair::ATVWidget()
{
	if (myPlayer->GetNetMode() == NM_Client && myPlayer->GetLocalRole() == ROLE_AutonomousProxy)
		AddToScreen(GetOwningLocalPlayer(), 0);
}
