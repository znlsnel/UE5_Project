// Fill out your copyright notice in the Description page of Project Settings.


#include "BarricadeItem.h"

ABarricadeItem::ABarricadeItem() : Super()
{
	Tags.Add("Barricade");
	Tags.Add("EnemysTarget");
}


void ABarricadeItem::BeginPlay()
{
	Super::BeginPlay();

	// OverlapCollision Init
	Tags.Add("Barricade");
	Tags.Add("EnemysTarget");
}

