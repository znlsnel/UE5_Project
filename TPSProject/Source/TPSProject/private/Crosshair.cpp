// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"
#include "TPSPlayer.h"

void UCrosshair::Initialization(ATPSPlayer* player)
{
	AddToViewport();
	myPlayer = player;

	
}
