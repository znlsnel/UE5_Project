// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "ScreenUI.h"
#include "InventorySlotPopup.h"
#include "Inventory.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

bool UPlayerUI::isInventoryOpen()
{
	return screenUI->bOpenInventory;
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

void UPlayerUI::GetMouseInput()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();

	UWidget* tempWidget = screenUI->inventory->InventorySlotPopup;

	if (tempWidget && tempWidget->IsVisible())
	{
		FGeometry Geometry = tempWidget->GetCachedGeometry();

		if (!Geometry.IsUnderLocation(MousePosition))
			screenUI->inventory->DestructPopup();
			
		
	}
	
}



void UPlayerUI::ToggleInventory()
{
	screenUI->ToggleInventory();
}
