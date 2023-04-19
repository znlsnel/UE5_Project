// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "ScreenUI.h"

void UPlayerUI::InitializeComponent()
{
	Super::InitializeComponent();

	me->onInputBindingDelegate.AddUObject(this, &UPlayerUI::SetupInputBinding);

}

void UPlayerUI::BeginPlay()
{
	Super::BeginPlay();
	screenUI = Cast<UScreenUI>(CreateWidget(GetWorld(), ScreenUIFactory));
	screenUI->Initialization(me);
}

void UPlayerUI::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &UPlayerUI::ToggleInventory);
}

void UPlayerUI::ToggleInventory()
{
	screenUI->ToggleInventory();
}
