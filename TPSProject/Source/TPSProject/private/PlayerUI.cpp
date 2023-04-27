// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "ScreenUI.h"
#include "InventorySlotPopup.h"
#include "Inventory.h"
#include "Crosshair.h"
#include "StoreActor.h"
#include "StoreUI.h"

#include <Blueprint/UserWidget.h>
#include <Components/WidgetComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

bool UPlayerUI::isInventoryOpen()
{
	return screenUI->bOpenInventory;
}

void UPlayerUI::InitializeComponent()
{
	Super::InitializeComponent();
	screenUI = Cast<UScreenUI>(CreateWidget(GetWorld(), ScreenUIFactory));
	screenUI->Initialization(me);

	crosshair = Cast<UCrosshair>(CreateWidget(GetWorld(), crosshairFactory));
	if (crosshair->IsValidLowLevel()) crosshair->Initialization(me);

	storeActor = GetWorld()->SpawnActor<AStoreActor>();
	storeUI = CreateWidget<UStoreUI>(GetWorld(), storeUIFactory);
	storeUI->myPlayer = me;
	storeActor->storeUI = storeUI;
}

void UPlayerUI::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPlayerUI::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &UPlayerUI::ToggleInventory);
}

void UPlayerUI::GetMouseInput()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	UWidget* tempWidget = screenUI->inventory->InventorySlotPopup;

	if (tempWidget && tempWidget->IsVisible())
	{
		FGeometry Geometry = tempWidget->GetCachedGeometry();

		if (!Geometry.IsUnderLocation(MousePosition))
			screenUI->inventory->DisablePopup();
	}

}



void UPlayerUI::ToggleInventory()
{
	screenUI->ToggleInventory();
}
