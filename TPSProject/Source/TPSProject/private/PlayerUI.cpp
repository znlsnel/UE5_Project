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
#include "PlayerAbilityComp.h"
#include "playerHpEffect.h"
#include "StatueHpWidget.h"
#include "StatueAbilityWidget.h"
#include "ExcOptionWidget.h"
#include "StartMenu.h"

#include <LevelSequence/Public/LevelSequence.h>
#include <LevelSequence/Public/LevelSequencePlayer.h>
#include <LevelSequence/Public/LevelSequenceActor.h>
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
	PlayerInputComponent->BindAction(TEXT("OptionScreen"), IE_Pressed, this, &UPlayerUI::OnOffOptionWidget);

	PlayerInputComponent->BindAction(TEXT("FirstSkillSlot"), IE_Pressed, screenUI, &UScreenUI::UseSkillSlot<SkillType::IceAttack>);
	PlayerInputComponent->BindAction(TEXT("SecondSkillSlot"), IE_Pressed, screenUI, &UScreenUI::UseSkillSlot<SkillType::LightningStrike>);
	PlayerInputComponent->BindAction(TEXT("ThirdSkillSlot"), IE_Pressed, screenUI, &UScreenUI::UseSkillSlot<SkillType::Healing>);
	PlayerInputComponent->BindAction(TEXT("FourthSkillSlot"), IE_Pressed, screenUI, &UScreenUI::UseSkillSlot<SkillType::FireStorm>);

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
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//APlayerController* pc = GetWorld()->GetFirstPlayerController();

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
		Cast<UPlayerMove>(me->playerMove)->turnValue = 0.f;
		Cast<UPlayerMove>(me->playerMove)->lookUpValue = 0.f;
		if (weaponSelectUI->isOpen == false)
			weaponSelectUI->WidgetOn();
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

void UPlayerUI::InitPlayerUI()
{
	if (sequencePlayer)
		sequencePlayer->Stop();

	screenUI->SetVisibility(ESlateVisibility::Visible);
	crosshair->SetVisibility(ESlateVisibility::Visible);
	StatueHpWidget->InitHpWidget();

}

void UPlayerUI::InitializeWidgets()
{

	screenUI = Cast<UScreenUI>(CreateWidget(GetWorld(), ScreenUIFactory));
	StatueHpWidget = Cast<UStatueHpWidget>(CreateWidget(GetWorld(), StatueHpFactory));

	screenUI->Initialization(me);

	crosshair = Cast<UCrosshair>(CreateWidget(GetWorld(), crosshairFactory));
	if (crosshair) crosshair->Initialization(me);
	weaponSelectUI = Cast<UWeaponUI>(CreateWidget(GetWorld(), weaponUIFactory));
	statueAbilityWidget = Cast<UStatueAbilityWidget>(CreateWidget(GetWorld(), statueAbilityWidgetFactory));
	excOptionWidget = Cast< UExcOptionWidget>(CreateWidget(GetWorld(), excOptionWidgetFactory));
	startMenu = Cast< UStartMenu>(CreateWidget(GetWorld(), startMenuFactory));
}

void UPlayerUI::OnOffOptionWidget()
{
	if (excOptionWidget == nullptr) return;

	if (excOptionWidget->IsInViewport()) {
		excOptionWidget->CloseWidget();
	}
	else {
		excOptionWidget->OpenWidget();

	}


}

void UPlayerUI::ATVWidgets()
{
	if (StatueHpWidget) {
		StatueHpWidget->AddToViewport();
	}

	if (crosshair) {
		crosshair->ATVWidget();
	}

	if (screenUI) {
		screenUI->ATVWidget();
	}

	if (weaponSelectUI && weaponSelectUI->IsInViewport() == false) {
		weaponSelectUI->AddToViewport();
	}


}



void UPlayerUI::ToggleInventory()
{
		screenUI->ToggleInventory();
}

// true -> On, false -> Off
void UPlayerUI::ToggleInventory(bool OnOff)
{
	screenUI->ToggleInventory(OnOff);
}

void UPlayerUI::GameOver()
{
	if (IsValid(MySequenceActor) == false)
		MySequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass());

	FMovieSceneSequencePlaybackSettings set;
	sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), gameOverSequence, set, MySequenceActor);

	sequencePlayer->Play();
	GetWorld()->GetTimerManager().SetTimer(GameOverTimer, FTimerDelegate::CreateLambda([&]() {
		GameOverSequencePauseEvent();
		}), 5.f, false);
	screenUI->SetVisibility(ESlateVisibility::Collapsed);
	crosshair->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerUI::GameOverSequencePauseEvent()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Stop!"));
	startMenu->AddToViewport();
	startMenu->OpenWidget(true);
}
