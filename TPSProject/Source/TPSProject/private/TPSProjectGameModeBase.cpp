// Copyright Epic Games, Inc. All Rights Reserved.



#include "TPSProjectGameModeBase.h"
#include "PlayerUI.h"
#include "Weapon.h"
#include "Crosshair.h"
#include "ScreenUI.h"
#include "Inventory.h"
#include "TPSPlayer.h"
#include "Doomstone.h"
#include "ItemStoreUI.h"
#include "WeaponData.h"
#include "InventorySlot.h"
#include "MySaveGame.h"
#include "BuildableItem.h"
#include "EnemyManager.h"

#include<Blueprint/UserWidget.h>
#include <Serialization/JsonWriter.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/GameState.h>
#include <GameFramework/PlayerController.h>
#include <Components/DirectionalLightComponent.h>
#include <Engine/DirectionalLight.h>

ATPSProjectGameModeBase::ATPSProjectGameModeBase()
{
}

ATPSProjectGameModeBase::~ATPSProjectGameModeBase()
{
	FString JsonString;
	FString filepath = FPaths::ProjectContentDir() + "\\Assets\\Json\\SaveFile.json";
	FFileHelper::LoadFileToString(JsonString, *filepath);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> JsonValueArray = JsonObject->GetArrayField(TEXT("SaveFiles"));

		for (auto jsonValue : JsonValueArray) {
			TSharedPtr<FJsonObject> jsonValueObject = jsonValue->AsObject();

			jsonValueObject->SetStringField("FirstSlot", "");
			jsonValueObject->SetNumberField("FirstSlotDay", 0);

			jsonValueObject->SetStringField("SecondSlot", "");
			jsonValueObject->SetNumberField("SecondSlotDay", 0);

			jsonValueObject->SetStringField("ThirdSlot", "");
			jsonValueObject->SetNumberField("ThirdSlotDay", 0);
		}
	}

	FString jsonFormattedString;
	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&jsonFormattedString);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), jsonWriter))
	{
		FFileHelper::SaveStringToFile(jsonFormattedString, *filepath);
	}
}

void ATPSProjectGameModeBase::InitGame(const FString& mapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(mapName, Options, ErrorMessage);
	//FString str = mapName + "  " + Options + "  " + ErrorMessage;

	//UKismetSystemLibrary::PrintString(GetWorld(), str);
	// Create and load a LocalPlayer
	loadingScreen = CreateWidget(GetWorld(), loadingScreenFactory);

}

void ATPSProjectGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ATPSProjectGameModeBase::BeginPlay()
{
	GetWorld()->Exec(GetWorld(), TEXT("DisableAllScreenMessages"));
	Super::BeginPlay();

	EnemyManager = Cast<AEnemyManager>(GetWorld()->SpawnActor(enemyManagerFactory));
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ATPSProjectGameModeBase::SaveGame(FString SaveSlotName, int slotNumber)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	SaveGameInstance->SaveFileName = SaveSlotName;
	SaveFileDuplicate(SaveGameInstance);

	FString preSaveFileName = GetSlotName(slotNumber);
	if (preSaveFileName != "")
		UGameplayStatics::DeleteGameInSlot(preSaveFileName, slotNumber);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, slotNumber);

	UpdateJsonFile(SaveSlotName, slotNumber, SaveGameInstance->currRound);
}

bool ATPSProjectGameModeBase::LoadGame(int slotNumber)
{
	FString slotName = ConvertSlotNumberToString(slotNumber);
	FString PreSaveFileName = GetSlotName(slotNumber);
	if (PreSaveFileName == "")
		return false;

	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(PreSaveFileName, slotNumber));

	SaveFileDuplicate(LoadGameInstance, true);
	EnemyManager->ResetEnemy();
	EnemyManager->PlayGameAudio(false, true);


	ADirectionalLight* tempLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));

	if (IsValid(tempLight))
		tempLight->SetActorRotation(FRotator(-89, 0, 0));
	
	if (UGameplayStatics::IsGamePaused(GetWorld()))
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	OpenLoadingScreen();

	return true;
}

void ATPSProjectGameModeBase::SaveFileDuplicate(UMySaveGame* saveGame, bool Load)
{
	if (saveGame == nullptr) {
		return;
	}

	if (Load) {
		myPlayer->Grace = saveGame->playerGrace;

		myPlayer->Mineral = saveGame->playerMineral;
		myPlayer->hp = saveGame->playerHp;
		myPlayer->UpdateHeartSound();
		myPlayer->SetActorLocation(saveGame->Location);
		myPlayer->playerFire->SetOwnWeapons(saveGame->ownWeapon);

		myPlayer->abilityComp->SetSkillInfoArr(saveGame->skillInfos);
		myPlayer->playerUI->statueAbilityWidget->SetGetStatueAbilityArr(saveGame->statueAbilitys, saveGame->abilityRepairRate, true, saveGame->statueHp);
		myPlayer->playerUI->screenUI->inventory->SetInventorySlot(saveGame->InventoryItemID);
		myPlayer->myAbilityWidget->SetAbilityInfo(saveGame->upgradeInfo);

		currRound = saveGame->currRound;

		myPlayer->playerUI->InitPlayerUI();
		SetFieldItem(saveGame->FieldItemID);
	}

	else {
		saveGame->playerGrace = myPlayer->Grace;
		saveGame->playerMineral = myPlayer->Mineral;
		saveGame->playerHp = myPlayer->hp;
		saveGame->ownWeapon = myPlayer->playerFire->GetOwnWeapons();

		saveGame->Location = myPlayer->GetActorLocation();
		myPlayer->playerUI->statueAbilityWidget->SetGetStatueAbilityArr(saveGame->statueAbilitys, saveGame->abilityRepairRate, false, saveGame->statueHp);

		saveGame->skillInfos = myPlayer->abilityComp->GetSkillInfoArr();


		saveGame->upgradeInfo = myPlayer->myAbilityWidget->GetAbilityInfo();
		myPlayer->playerUI->screenUI->inventory->GetInventorySlot(saveGame->InventoryItemID);

		saveGame->currRound = currRound;
		GetFieldItem(saveGame->FieldItemID);

	}
}

void ATPSProjectGameModeBase::UpdateJsonFile(FString SaveSlotName, int slotNumber, int currDay)
{
	FString JsonString;
	FString filepath = FPaths::ProjectContentDir() + "\\Assets\\Json\\SaveFile.json";
	FFileHelper::LoadFileToString(JsonString, *filepath);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> JsonValueArray = JsonObject->GetArrayField(TEXT("SaveFiles"));

		for (auto jsonValue : JsonValueArray) {
			TSharedPtr<FJsonObject> jsonValueObject = jsonValue->AsObject();

			FString slotName = ConvertSlotNumberToString(slotNumber);
			FString slotDay = slotName + "Day";

			jsonValueObject->SetStringField(slotName, SaveSlotName);
			jsonValueObject->SetNumberField(slotDay, currDay);

			firstSaveSlotName = jsonValueObject->GetStringField(TEXT("FirstSlot"));
			SecondSaveSlotName = jsonValueObject->GetStringField(TEXT("SecondSlot"));
			ThirdSaveSlotName = jsonValueObject->GetStringField(TEXT("ThirdSlot"));

			UKismetSystemLibrary::PrintString(GetWorld(), slotName + " : " + SaveSlotName);
		}
	}

	FString jsonFormattedString;
	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create(&jsonFormattedString);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), jsonWriter))
	{
		FFileHelper::SaveStringToFile(jsonFormattedString, *filepath);
	}
}

FString ATPSProjectGameModeBase::ConvertSlotNumberToString(int slotNumber)
{
	FString slotName = "";
	if (slotNumber == 1)
		slotName = "FirstSlot";
	else if (slotNumber == 2)
		slotName = "SecondSlot";
	else
		slotName = "ThirdSlot";

	return slotName;
}

FString ATPSProjectGameModeBase::GetSlotName(int SlotNumber)
{
	switch (SlotNumber) {
	case 1:
		return firstSaveSlotName;
	case 2:
		return SecondSaveSlotName;
	case 3:
		return ThirdSaveSlotName;
	}
	return "";
}

void ATPSProjectGameModeBase::GetFieldItem(TArray<struct FFieldItem>& fieldItems)
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildableItem::StaticClass(), actors);

	fieldItems.Empty();
	for (auto actor : actors) {
		if (actor->GetClass()->IsChildOf(ABuildableItem::StaticClass())) {
			if (actor->IsHidden()) return;
			ABuildableItem* tempItem = Cast<ABuildableItem>(actor);
			FFieldItem item;
			item.Location = tempItem->GetActorLocation();
			item.rotation = tempItem->GetActorRotation();

			EItemID itemId = (EItemID)tempItem->buildableItemType;

			item.ItemID = itemId;
			fieldItems.Add(item);
		}
	}
}

void ATPSProjectGameModeBase::SetFieldItem(TArray<struct FFieldItem>& fieldItems)
{
	TArray<AActor*> actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildableItem::StaticClass(), actors);

	for (auto actor : actors) {
		if (actor->GetClass()->IsChildOf(ABuildableItem::StaticClass())) {
			actor->Destroy();
		}
	}


	for (auto itemInfo : fieldItems) {
		myPlayer->CreateItem(loadActors, itemInfo.ItemID);

		for (auto tempItem : loadActors) {
			if (IsValid(tempItem)) {
				tempItem->SetActorLocation(itemInfo.Location);
				tempItem->SetActorRotation(itemInfo.rotation);
			}
		}
		loadActors.Empty();
	}

}

void ATPSProjectGameModeBase::OpenLoadingScreen()
{
	loadingScreen->AddToViewport();

	GetWorld()->GetTimerManager().SetTimer(loadingScreenTimer, FTimerDelegate::CreateLambda(
		[&]() {
			loadingScreen->RemoveFromParent();
		}), 3.f, false);
}


