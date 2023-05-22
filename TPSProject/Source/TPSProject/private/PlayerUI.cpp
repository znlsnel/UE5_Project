// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "ScreenUI.h"
#include "InventorySlotPopup.h"
#include "Inventory.h"
#include "Crosshair.h"
#include "StoreActor.h"
#include "StoreUI.h"
#include "WeaponUI.h"
#include "PlayerMove.h"

#include <Blueprint/UserWidget.h>
#include <Components/WidgetComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Net/UnrealNetwork.h>
#include <GameFramework/PlayerState.h>
#include <Kismet/GameplayStatics.h>


void UPlayerUI::InitializeComponent()
{
	Super::InitializeComponent();
	InitializeWidgets();
}

void UPlayerUI::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPlayerUI::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &UPlayerUI::ToggleInventory);
	PlayerInputComponent->BindAction(TEXT("MouseToggle"), IE_Pressed, this, &UPlayerUI::ToggleMouse);

	PlayerInputComponent->BindAction(TEXT("WeaponSelectUI"), IE_Pressed, this, &UPlayerUI::ATVWeaponSelectUI<true>);
	PlayerInputComponent->BindAction(TEXT("WeaponSelectUI"), IE_Released, this, &UPlayerUI::ATVWeaponSelectUI<false>);

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

void UPlayerUI::ToggleMouse()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	if (IsMouseActive)
	{
		pc->bShowMouseCursor = false;
		pc->bEnableClickEvents = false;
		pc->bEnableMouseOverEvents = false;
		IsMouseActive = false;
	}
	else
	{
		int32 x;
		int32 y;
		pc->GetViewportSize(x, y);
		pc->SetMouseLocation(x / 2, y / 2);
		
		pc->bShowMouseCursor = true;
		pc->bEnableClickEvents = true;
		pc->bEnableMouseOverEvents = true;
		IsMouseActive = true;
	}

}

void UPlayerUI::ToggleMouse(bool ActiveMouse)
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	if (ActiveMouse == false)
	{
		pc->SetShowMouseCursor(false);

		pc->bShowMouseCursor = false;
		pc->bEnableClickEvents = false;
		pc->bEnableMouseOverEvents = false;
		IsMouseActive = false;
	}
	else
	{
		pc->SetShowMouseCursor(true);

		int32 x;
		int32 y;
		pc->GetViewportSize(x, y);
		pc->SetMouseLocation(x/2 , y/2);

		pc->bShowMouseCursor = true;
		pc->bEnableClickEvents = true;
		pc->bEnableMouseOverEvents = true;
		IsMouseActive = true;
	}
}

void UPlayerUI::ATVWeaponSelectUI(bool addToView)
{
	if (IsValid(weaponSelectUI) == false) return;

	//ToggleMouse(addToView);
	if (addToView)
	{
		if (weaponSelectUI->isOpen == false)
			weaponSelectUI->WidgetOn();
		Cast<UPlayerMove>(me->playerMove)->turnValue = 0.f;
		Cast<UPlayerMove>(me->playerMove)->lookUpValue = 0.f;
		IsMouseActive = true;
	}
	else
	{
		if (weaponSelectUI->isOpen)
			weaponSelectUI->WidgetOff();
		IsMouseActive = false;
	}
	
}

void UPlayerUI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UPlayerUI, screenUI);
	//DOREPLIFETIME(UPlayerUI, crosshair);
	//DOREPLIFETIME(UPlayerUI, storeUI);
}

void UPlayerUI::InitializeWidgets()
{
	// Client
	if (me->GetRemoteRole() == ROLE_Authority)
	{
		InitializeWidgets_Client();
	}
	// Server
	else if (me->GetLocalRole() == ROLE_Authority)
	{
		InitializeWidgets_Server();
	}
	
}

void UPlayerUI::ATVWidgets_Implementation()
{
	if (screenUI)	{
		screenUI->ATVWidget();
	}
	if (crosshair) {
		crosshair->ATVWidget();
	}
	if (weaponSelectUI) {
		weaponSelectUI->AddToViewport();

	}

}

void UPlayerUI::InitializeWidgets_Client_Implementation()
{
	APlayerController* tempPctrl = nullptr;
	for (FConstPlayerControllerIterator Iterator =  GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		
		if (Iterator->Get()->IsLocalPlayerController())
		{
			tempPctrl = Iterator->Get();
			break;
		}
	}


	screenUI = Cast<UScreenUI>(CreateWidget(tempPctrl, ScreenUIFactory));

	screenUI->Initialization(me);

	crosshair = Cast<UCrosshair>(CreateWidget(GetWorld(), crosshairFactory));
	if (crosshair->IsValidLowLevel()) crosshair->Initialization(me);

	weaponSelectUI = Cast<UWeaponUI>(CreateWidget(GetWorld(), weaponUIFactory));

}

void UPlayerUI::InitializeWidgets_Server_Implementation()
{
	screenUI = Cast<UScreenUI>(CreateWidget(GetWorld(), ScreenUIFactory));

	screenUI->Initialization(me);

	crosshair = Cast<UCrosshair>(CreateWidget(GetWorld(), crosshairFactory));
	if (crosshair->IsValidLowLevel()) crosshair->Initialization(me);

}

void UPlayerUI::ToggleInventory()
{
	screenUI->ToggleInventory();
}
