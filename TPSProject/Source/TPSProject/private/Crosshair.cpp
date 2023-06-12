// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"
#include "TPSPlayer.h"
#include <Kismet/KismetSystemLibrary.h>

#include <Net/UnrealNetwork.h>
void UCrosshair::Initialization(ATPSPlayer* player)
{
	
	myPlayer = player;

}

void UCrosshair::ATVWidget()
{
	if (IsInViewport()) return;
	AddToViewport();
}
