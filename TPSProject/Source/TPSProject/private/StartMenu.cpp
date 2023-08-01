// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenu.h"
#include "TPSProjectGameModeBase.h"

#include <LevelSequence/Public/LevelSequencePlayer.h>
#include <Components/PanelWidget.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/AudioComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/Level.h>

void UStartMenu::NativeConstruct()
{
	myGameMode = Cast<ATPSProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool UStartMenu::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	OpenWidget(false);
	StartGameButton->OnClicked.AddDynamic(this, &UStartMenu::ClickStartGameButton);
	ContinueGameButton->OnClicked.AddDynamic(this, &UStartMenu::ClickContinueGameButton);
	QuitGameButton->OnClicked.AddDynamic(this, &UStartMenu::ClickQuitGameButton);

	FirstSlot->OnClicked.AddDynamic(this, &UStartMenu::ClickFirstSlotButton);
	SecondSlot->OnClicked.AddDynamic(this, &UStartMenu::ClickSecondSlotButton);
	ThirdSlot->OnClicked.AddDynamic(this, &UStartMenu::ClickThirdSlotButton);
	LoadButton->OnClicked.AddDynamic(this, &UStartMenu::ClickLoadButton);

	StartMenuButton->OnClicked.AddDynamic(this, &UStartMenu::ClickStartMenuButton);

	return true;
}

void UStartMenu::UpdateSlotName()
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

void UStartMenu::ClickStartGameButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Project_T"));
	if (startMusicAudio && startMusicAudio->IsPlaying())
		startMusicAudio->Stop();
	RemoveFromParent();
}

void UStartMenu::ClickLoadButton()
{	
	int slotNum = 1;

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
	if (startMusicAudio && startMusicAudio->IsPlaying())
		startMusicAudio->Stop();

	if (myGameMode->LoadGame(slotNum) ){
		RemoveFromParent();
	}
}

void UStartMenu::ClickContinueGameButton()
{
	if (SaveSlotPanel->RenderOpacity == 1.f) {
		SaveSlotPanel->SetRenderOpacity(0.f);
	}
	else {
		SaveSlotPanel->SetRenderOpacity(1.f);

	}

}

void UStartMenu::ClickQuitGameButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UStartMenu::ClickFirstSlotButton()
{
	if (currSelectSlot == ESelectSlot::FirstSlot)
		currSelectSlot = ESelectSlot::None;
	else
		currSelectSlot = ESelectSlot::FirstSlot;
	UpdateSelectArrow();
}

void UStartMenu::ClickSecondSlotButton()
{
	if (currSelectSlot == ESelectSlot::SecondSlot)
		currSelectSlot = ESelectSlot::None;
	else
		currSelectSlot = ESelectSlot::SecondSlot;
	UpdateSelectArrow();
}

void UStartMenu::ClickThirdSlotButton()
{
	if (currSelectSlot == ESelectSlot::ThirdSlot)
		currSelectSlot = ESelectSlot::None;
	else
		currSelectSlot = ESelectSlot::ThirdSlot;
	UpdateSelectArrow();
}

void UStartMenu::ClickStartMenuButton()
{
	OpenWidget(false);

}

void UStartMenu::UpdateSelectArrow()
{
	switch (currSelectSlot) {
	case ESelectSlot::FirstSlot: {
		FirstSlotArrow->SetRenderOpacity(1.f);
		SecondSlotArrow->SetRenderOpacity(0.f);
		ThirdSlotArrow->SetRenderOpacity(0.f);
	}
		break;
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

void UStartMenu::OpenWidget(bool GameOver, ULevelSequencePlayer* gameOverSequencePlayer)
{
	if (GameOver) {
		GameOverPanel->SetRenderScale(FVector2D(1));
		MainPanel->SetRenderScale(FVector2D(0));
		BackGroundImage->SetRenderOpacity(0.7f);
		sequencePlayer = gameOverSequencePlayer;
	}
	else {
		GameOverPanel->SetRenderScale(FVector2D(0));
		MainPanel->SetRenderScale(FVector2D(1));
		BackGroundImage->SetRenderOpacity(1.f);

		if (IsInViewport()) {
			if (startMusicAudio == nullptr)
				startMusicAudio = UGameplayStatics::SpawnSound2D(GetWorld(), startMusic);
			if (startMusicAudio && startMusicAudio->IsPlaying() == false)
				startMusicAudio->Play();
		}
	}
	UpdateSlotName();
	UpdateSelectArrow();
}

