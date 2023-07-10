// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSProjectGameModeBase.h"
#include "TPSPlayer.h"
#include "EnemyManager.h"
#include "MySaveGame.h"

#include "Inventory.h"
#include "ScreenUI.h"
#include "InventorySlot.h"
#include "PlayerUI.h"
#include "BuildableItem.h"
#include "Weapon.h"
#include "WeaponData.h"

#include <Serialization/JsonWriter.h>
#include <GameFramework/GameState.h>
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include<Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>


#include <Net/UnrealNetwork.h>

ATPSProjectGameModeBase::ATPSProjectGameModeBase()
{

}

void ATPSProjectGameModeBase::InitGame(const FString& mapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(mapName, Options, ErrorMessage);

	// Create and load a LocalPlayer

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

	UGameplayStatics::OpenLevel(GetWorld(), FName("Project_T"), false);
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(PreSaveFileName, slotNumber));

	SaveFileDuplicate(LoadGameInstance, true);
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
		myPlayer->myAbilityWidget->SetAbilityInfo(saveGame->upgradeInfo);
		myPlayer->SetActorLocation(saveGame->Location);
		myPlayer->playerFire->SetOwnWeapons(saveGame->ownWeapon);

		myPlayer->abilityComp->SetSkillInfoArr(saveGame->skillInfos);
		myPlayer->playerUI->statueAbilityWidget->SetGetStatueAbilityArr(saveGame->statueAbilitys, saveGame->abilityRepairRate, true);
		myPlayer->playerUI->screenUI->inventory->SetInventorySlot(saveGame->InventoryItemID);

		myPlayer->currRound = saveGame->currRound;
		myPlayer->playerUI->screenUI->currDay = saveGame->currRound;
		SetFieldItem(saveGame->FieldItemID);
	}

	else {
		saveGame->playerGrace = myPlayer->Grace;
		saveGame->playerMineral = myPlayer->Mineral;
		saveGame->ownWeapon = myPlayer->playerFire->GetOwnWeapons();

		saveGame->Location = myPlayer->GetActorLocation();
		saveGame->skillInfos = myPlayer->abilityComp->GetSkillInfoArr();

		myPlayer->playerUI->statueAbilityWidget->SetGetStatueAbilityArr(saveGame->statueAbilitys, saveGame->abilityRepairRate, false);

		saveGame->upgradeInfo = myPlayer->myAbilityWidget->GetAbilityInfo();
		myPlayer->playerUI->screenUI->inventory->GetInventorySlot(saveGame->InventoryItemID);

		saveGame->currRound = myPlayer->currRound;
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
			jsonValueObject->SetNumberField(slotDay, currDay + 1);

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

			int itemId = (int)tempItem->buildableItemType + 1000;

			item.ItemID = itemId;
			fieldItems.Add(item);
		}
	}
}

void ATPSProjectGameModeBase::SetFieldItem(TArray<struct FFieldItem>& fieldItems)
{
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


