// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilityComp.h"
#include "TPSPlayer.h"


#include <Serialization/JsonReader.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UPlayerAbilityComp::UPlayerAbilityComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Load Json
	LoadJsonFile();


	// ...
}


void UPlayerAbilityComp::LoadJsonFile()
{
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectContentDir() + "\\Assets\\Json\\SkillInfoJson.json"));
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> JsonValueArray = JsonObject->GetArrayField(TEXT("FSkillInfo"));

		for (auto jsonValue : JsonValueArray) {
			TSharedPtr<FJsonObject> jsonValueObject = jsonValue->AsObject();

			FSkillInfo* tempSkillInfo = new FSkillInfo();
			tempSkillInfo->SkillName = jsonValueObject->GetStringField(TEXT("SkillName"));
			tempSkillInfo->SkillInfo = jsonValueObject->GetStringField(TEXT("SkillInfo"));

			TArray<TSharedPtr<FJsonValue>> powerValues = jsonValueObject->GetArrayField(TEXT("powerValues"));
			for (auto powerValue : powerValues) {
				int tempN = FMath::CeilToInt(powerValue.Get()->AsNumber());
				tempSkillInfo->powerValues.Add(tempN);
			}

			TArray<TSharedPtr<FJsonValue>> coolDownValues = jsonValueObject->GetArrayField(TEXT("coolDownValues"));
			for (auto coolDownValue : coolDownValues) {
				tempSkillInfo->coolDownValues.Add(coolDownValue.Get()->AsNumber());
			}

			tempSkillInfo->SectionType = ConvertSectionType(jsonValueObject->GetStringField(TEXT("SectionType")));
			tempSkillInfo->skillType = ConvertSkillType(jsonValueObject->GetStringField(TEXT("SkillType")));
			tempSkillInfo->isAbility = jsonValueObject->GetBoolField(TEXT("isAbility"));


			skillInfos.Add(tempSkillInfo);
		}
	}
}


// Called every frame
void UPlayerAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FSkillInfo* UPlayerAbilityComp::GetSkillInfo(SkillType type)
{
	for (auto skillInfo : skillInfos)
	{
		if (skillInfo->skillType == type)
			return skillInfo;
	}
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No SkillInfo"));
	return nullptr;
}

SkillSectionType UPlayerAbilityComp::ConvertSectionType(FString type)
{
	SkillSectionType tempType = SkillSectionType::None;
	if (type == "thirdSkillSection")
		tempType = SkillSectionType::thirdSkillSection;
	else if (type == "firstSkillSection")
		tempType = SkillSectionType::firstSkillSection;
	else if (type == "secondSkillSection")
		tempType = SkillSectionType::secondSkillSection;
	else if (type == "fourthSkillSection")
		tempType = SkillSectionType::fourthSkillSection;
	return tempType;
}

SkillType UPlayerAbilityComp::ConvertSkillType(FString type)
{
	SkillType tempType = SkillType::None;
	if (type == "swordProficiency")
		tempType = SkillType::swordProficiency;
	else if (type == "bowProficiency")
		tempType = SkillType::bowProficiency;
	else if (type == "gunProficiency")
		tempType = SkillType::gunProficiency;
	else if (type == "HpUpgrade")
		tempType = SkillType::HpUpgrade;
	else if (type == "HpNaturalHealing")
		tempType = SkillType::HpNaturalHealing;
	else if (type == "DoubleAttack")
		tempType = SkillType::DoubleAttack;
	else if (type == "LuckyShot")
		tempType = SkillType::LuckyShot;
	else if (type == "CoinBoost")
		tempType = SkillType::CoinBoost;
	else if (type == "IceAttack")
		tempType = SkillType::IceAttack;
	else if (type == "LightningStrike")
		tempType = SkillType::LightningStrike;
	else if (type == "Healing")
		tempType = SkillType::Healing;
	else if (type == "FireStorm")
		tempType = SkillType::FireStorm;

	return tempType;
}

