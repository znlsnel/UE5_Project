// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseComponent.h"


void UPlayerBaseComponent::InitializeComponent()
{
	me = Cast<ATPSPlayer>(GetOwner());
	moveComp = me->GetCharacterMovement();

	me->onInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);

}

// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

}



