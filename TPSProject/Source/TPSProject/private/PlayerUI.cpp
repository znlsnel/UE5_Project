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
#include <Net/UnrealNetwork.h>
#include <GameFramework/PlayerState.h>
#include <Kismet/GameplayStatics.h>

bool UPlayerUI::isInventoryOpen()
{
	if (screenUI == nullptr) return false;
	return screenUI->bOpenInventory;
}

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

void UPlayerUI::ATVWidgets()
{
	if (screenUI) screenUI->ATVWidget();
	if (crosshair) crosshair->ATVWidget();
	if (storeActor) storeActor->SetHidden(false);
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

	storeActor = GetWorld()->SpawnActor<AStoreActor>();

	storeUI = CreateWidget<UStoreUI>(tempPctrl, storeUIFactory);
	if (IsValid(storeUI)) storeUI->myPlayer = me;

	storeActor->storeUI = storeUI;
	storeActor->SetHidden(true);

}

void UPlayerUI::InitializeWidgets_Server_Implementation()
{
	screenUI = Cast<UScreenUI>(CreateWidget(GetWorld(), ScreenUIFactory));

	screenUI->Initialization(me);

	crosshair = Cast<UCrosshair>(CreateWidget(GetWorld(), crosshairFactory));
	if (crosshair->IsValidLowLevel()) crosshair->Initialization(me);

	storeActor = GetWorld()->SpawnActor<AStoreActor>();

	storeUI = CreateWidget<UStoreUI>(GetWorld(), storeUIFactory);
	if (IsValid(storeUI)) storeUI->myPlayer = me;
	storeActor->storeUI = storeUI;

}

void UPlayerUI::ToggleInventory()
{
	screenUI->ToggleInventory();
}
