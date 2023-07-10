// Fill out your copyright notice in the Description page of Project Settings.


#include "ExcOptionWidget.h"
#include "MySaveGame.h"
#include "TPSPlayer.h"
#include "TPSProjectGameModeBase.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

void UExcOptionWidget::NativeConstruct()
{
	player = Cast <ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	myGameMode = Cast<ATPSProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

}

bool UExcOptionWidget::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	UpdateSelectArrow();
	UpdateSlotName();

	FirstSlot->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickFirstSlotButton);
	SecondSlot->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickSecondSlotButton);
	ThirdSlot->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickThirdSlotButton);

	SaveButton->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickSaveButton);
	LoadButton->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickLoadButton);

	SaveSlotOpenButton->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickSaveSlotOpenButton);
	CloseButton->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickCloseButton);
	CloseInputTextSlotButton->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickCloseInputTextSlotButton);
	InputTextSlotCheckButton->OnClicked.AddDynamic(this, &UExcOptionWidget::ClickInputTextSlotCheckButton);


	return true;
}

void UExcOptionWidget::OpenWidget()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(closeTimer))
		return;

	AddToViewport();
	PlayAnimation(OpenOption);
	
	if (player == nullptr)
		player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->isMovable = false;
	player->isRotatable = false;
}

void UExcOptionWidget::CloseWidget()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(closeTimer))
		return;

	PlayAnimation(CloseOption);
	if (isOpenSaveSlotPanel) {
		PlayAnimation(CloseSaveSlotPanel);
		isOpenSaveSlotPanel = false;
	}
	player->isMovable = true;
	player->isRotatable = true;

	GetWorld()->GetTimerManager().SetTimer(closeTimer, FTimerDelegate::CreateLambda(
		[&]() {
			RemoveFromParent();
		}), 1.f, false);
}

void UExcOptionWidget::ClickFirstSlotButton()
{
	if (currSelectSlot == ESelectSlot::FirstSlot)
		currSelectSlot = ESelectSlot::None;
	else
		currSelectSlot = ESelectSlot::FirstSlot;
	UpdateSelectArrow();
}

void UExcOptionWidget::ClickSecondSlotButton()
{
	if (currSelectSlot == ESelectSlot::SecondSlot)
		currSelectSlot = ESelectSlot::None;
	else
		currSelectSlot = ESelectSlot::SecondSlot;
	UpdateSelectArrow();
}

void UExcOptionWidget::ClickThirdSlotButton()
{
	if (currSelectSlot == ESelectSlot::ThirdSlot)
		currSelectSlot = ESelectSlot::None;
	else
		currSelectSlot = ESelectSlot::ThirdSlot;
	UpdateSelectArrow();
}

void UExcOptionWidget::ClickSaveButton()
{
	if (currSelectSlot != ESelectSlot::None && IsValid(OpenInputTextSlot))
		PlayAnimation(OpenInputTextSlot);

}

void UExcOptionWidget::ClickLoadButton()
{
	int slotNum = 0;

	switch (currSelectSlot) {
	case ESelectSlot::FirstSlot:
		slotNum = 1;
		break;
	case ESelectSlot::SecondSlot:
		slotNum = 2;
		break;
	case ESelectSlot::ThirdSlot:
		slotNum = 3;
		break;
	default:
		return;
	}

	myGameMode->LoadGame(slotNum);
}

void UExcOptionWidget::ClickSaveSlotOpenButton()
{
	if (IsPlayingAnimation())
		return;

	UWidgetAnimation* tempAnim = OpenSaveSlotPanel;
	if (isOpenSaveSlotPanel ) {
		tempAnim = CloseSaveSlotPanel;
	}
	isOpenSaveSlotPanel = isOpenSaveSlotPanel ? false : true;
	PlayAnimation(tempAnim);
	PlayAnimation(CloseInputTextSlot);
}

void UExcOptionWidget::ClickCloseButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UExcOptionWidget::ClickCloseInputTextSlotButton()
{
	if (IsValid(CloseInputTextSlot))
		PlayAnimation(CloseInputTextSlot);
}

void UExcOptionWidget::ClickInputTextSlotCheckButton()
{	
	if (InputText == "" || currSelectSlot == ESelectSlot::None) {
		return;
	}

	if (IsValid(CloseInputTextSlot))
		PlayAnimation(CloseInputTextSlot);
	if (myGameMode)
		myGameMode->SaveGame(InputText, (int)currSelectSlot);
	else
		return;

	UpdateSelectArrow(true);
	UpdateSlotName();
}

void UExcOptionWidget::UpdateSelectArrow(bool Initialize)
{
	if (Initialize)
		currSelectSlot = ESelectSlot::None;

	switch (currSelectSlot) {
	case ESelectSlot::FirstSlot: {
		FirstSlotArrow->SetRenderOpacity(1.f);
		SecondSlotArrow->SetRenderOpacity(0.f);
		ThirdSlotArrow->SetRenderOpacity(0.f);
		break;
	}
	case ESelectSlot::SecondSlot: {
		FirstSlotArrow->SetRenderOpacity(0.f);
		SecondSlotArrow->SetRenderOpacity(1.f);
		ThirdSlotArrow->SetRenderOpacity(0.f);
	}
				    break;
	case ESelectSlot::ThirdSlot: {
		FirstSlotArrow->SetRenderOpacity(0.f);
		SecondSlotArrow->SetRenderOpacity(0.f);
		ThirdSlotArrow->SetRenderOpacity(1.f);
	}
				   break;
	case ESelectSlot::None: {
		FirstSlotArrow->SetRenderOpacity(0.f);
		SecondSlotArrow->SetRenderOpacity(0.f);
		ThirdSlotArrow->SetRenderOpacity(0.f);
	}
			      break;
	}
	
}

void UExcOptionWidget::UpdateSlotName()
{
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectContentDir() + "\\Assets\\Json\\SaveFile.json"));
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> JsonValueArray = JsonObject->GetArrayField(TEXT("SaveFiles"));

		for (auto jsonValue : JsonValueArray) {
			TSharedPtr<FJsonObject> jsonValueObject = jsonValue->AsObject();

			FString tempName = "";
			tempName = jsonValueObject->GetStringField("FirstSlot");
			if (tempName == "")
				tempName = "None";
			FirstSaveSlotName = tempName;

			tempName = jsonValueObject->GetStringField("SecondSlot");
			if (tempName == "")
				tempName = "None";
			SecondSaveSlotName = tempName;

			tempName = jsonValueObject->GetStringField("ThirdSlot");
			if (tempName == "")
				tempName = "None";
			ThirdSaveSlotName = tempName;


			int day = 0;
			day = jsonValueObject->GetIntegerField("FirstSlotDay");
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("day : %d"), day ));
			if (day != 0)
				FirstSaveSlotDay = FString::Printf(TEXT(" : Day %d"), day);
			else
				FirstSaveSlotDay = "";

			day = jsonValueObject->GetIntegerField("SecondSlotDay");
			if (day != 0)
				SecondSaveSlotDay = FString::Printf(TEXT(" : Day %d"), day);
			else
				SecondSaveSlotDay = "";

			day = jsonValueObject->GetIntegerField("ThirdSlotDay");
			if (day != 0)
				ThirdSaveSlotDay = FString::Printf(TEXT(" : Day %d"), day);
			else
				ThirdSaveSlotDay = "";
		}
	}
}
