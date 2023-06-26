// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilityComp.h"
#include "TPSPlayer.h"
#include "Skill.h"
#include "ScreenUI.h"

#include <Serialization/JsonReader.h>
#include <Kismet/GameplayStatics.h>
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
			//UKismetSystemLibrary::PrintString(GetWorld(), tempSkillInfo->SkillInfo);

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


void UPlayerAbilityComp::BeginPlay()
{
	myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	for (int i = 0; i < 5; i++) {
		fireStorm.Add(GetWorld()->SpawnActor<ASkill>(FireStormFactory));
		HealStorm.Add(GetWorld()->SpawnActor<ASkill>(HealFactory));
		IceAttack.Add(GetWorld()->SpawnActor<ASkill>(IceAttackFactory));
		LightningStrike.Add(GetWorld()->SpawnActor<ASkill>(LightningStrikeFactory));
	}
}

// Called every frame
void UPlayerAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

bool UPlayerAbilityComp::GetMouseInput()
{
	if (currSkill == nullptr) 
		return false;

	if (currSkill->hitResult.bBlockingHit == false) 
		return true;

	int randSkill = FMath::RandRange(1, 3);

	if (myPlayer == nullptr)
		myPlayer = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (myPlayer) {
		myPlayer->PlayMontage(skillAnim, FName(FString::Printf(TEXT("Skill_%d"), randSkill)));
		isPlaySkillAnim = true;
	}
	else
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Player"));


	return true;
}

void UPlayerAbilityComp::SkillTrigger()
{
	currSkill->TriggerSkill();
	SetSkillTimer(currSkill->skillType);
	currSkill = nullptr;
}

void UPlayerAbilityComp::SetSkillTimer(SkillType type)
{
	switch (type)
	{
	case SkillType::IceAttack:
		IceAttackTime = currSkill->mySkill->coolDownValue;
		myScreenUI->IceAttackTimeText = FString::Printf(TEXT("%d"), (int)IceAttackTime);
		break;
	case SkillType::LightningStrike:
		LightningStrikeTime = currSkill->mySkill->coolDownValue;
		myScreenUI->LightningStrikeTimeText = FString::Printf(TEXT("%d"), (int)LightningStrikeTime);
		break;
	case SkillType::Healing:
		HealTime = currSkill->mySkill->coolDownValue;
		myScreenUI->HealTimeText = FString::Printf(TEXT("%d"), (int)HealTime);
		break;
	case SkillType::FireStorm:
		FireStormTime = currSkill->mySkill->coolDownValue;
		myScreenUI->FireStormTimeText = FString::Printf(TEXT("%d"), (int)FireStormTime);
		break;
	}
	if (GetWorld()->GetTimerManager().IsTimerActive(skillTimerHandle) == false)
		OperateSkillTimer();
}

void UPlayerAbilityComp::OperateSkillTimer()
{

	bool isAllZero = true;
	if (IceAttackTime > 0.f) {
		IceAttackTime -= 0.1f;
		if (FMath::Fmod(IceAttackTime, 1.0f) <= 0.1f) {
			if (IceAttackTime <= 0.1f) {
				myScreenUI->IceAttackTimeText = "";
				myScreenUI->ToggleSkillSlot(SkillType::IceAttack, false);
			}
			else {
				myScreenUI->IceAttackTimeText = FString::Printf(TEXT("%d"), (int)IceAttackTime);
			}
		}
		isAllZero = false;
	}

	if (LightningStrikeTime > 0.f) {
		LightningStrikeTime -= 0.1f;
		if (FMath::Fmod(LightningStrikeTime, 1.0f) <= 0.1f) {
			if (LightningStrikeTime <= 0.1f) {
				myScreenUI->LightningStrikeTimeText = "";
				myScreenUI->ToggleSkillSlot(SkillType::LightningStrike, false);
			}
			else {
				myScreenUI->LightningStrikeTimeText = FString::Printf(TEXT("%d"), (int)LightningStrikeTime);
			}
		}
		isAllZero = false;
	}

	if (HealTime > 0.f) {
		HealTime -= 0.1f;
		if (FMath::Fmod(HealTime, 1.0f) <= 0.1f) {
			if (HealTime <= 0.1f) {
				myScreenUI->HealTimeText = "";
				myScreenUI->ToggleSkillSlot(SkillType::Healing, false);
			}
			else {
				myScreenUI->HealTimeText = FString::Printf(TEXT("%d"), (int)HealTime);
			}
		}
		isAllZero = false;

	}

	if (FireStormTime > 0.f) {

		FireStormTime -= 0.1f;
		if (FMath::Fmod(FireStormTime, 1.0f) <= 0.05f) {
			if (FireStormTime <= 0.05f) {
				myScreenUI->FireStormTimeText = "";
				myScreenUI->ToggleSkillSlot(SkillType::FireStorm, false);
			}
			else {
				myScreenUI->FireStormTimeText = FString::Printf(TEXT("%d"), (int)FireStormTime);
			}
		}
		isAllZero = false;
	}

	if (myScreenUI->DashTime > 0.f) {
		myScreenUI->DashTime -= 0.1f;
		if (FMath::Fmod(myScreenUI->DashTime, 1.0f) <= 0.05f) {
			if (myScreenUI->DashTime <= 0.05f) {
				myScreenUI->DashTimeText = "";
				myScreenUI->ToggleSkillSlot(SkillType::FireStorm, false);
			}
			else {
				myScreenUI->DashTimeText = FString::Printf(TEXT("%d"), (int)myScreenUI->DashTime);
			}
		}
		isAllZero = false;

	}

	if (myScreenUI->ShieldTime > 0.f) {
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Shield : %f"), myScreenUI->ShieldTime));
		myScreenUI->ShieldTime -= 0.1f;
		if (FMath::Fmod(myScreenUI->ShieldTime, 1.0f) <= 0.05f) {
			if (myScreenUI->ShieldTime <= 0.05f) {
				myScreenUI->ShieldTimeText = "";
				myScreenUI->ToggleSkillSlot(SkillType::FireStorm, false);
			}
			else {
				myScreenUI->ShieldTimeText = FString::Printf(TEXT("%d"), (int)myScreenUI->ShieldTime);
			}
		}
		isAllZero = false;

	}
	GetWorld()->GetTimerManager().ClearTimer(skillTimerHandle);

	if (isAllZero == false) {
	GetWorld()->GetTimerManager().SetTimer(skillTimerHandle, this, &UPlayerAbilityComp::OperateSkillTimer, 0.1f, false);
	}
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

bool UPlayerAbilityComp::UseSkill(SkillType type)
{
	if (currSkill) 
	{
		currSkill->OnSkill(false);
		if (currSkill->skillType == type) {
			currSkill = nullptr;
			return false;
		}
	}

	currSkill = GetSkill(type);
	if (currSkill == nullptr) return false;
	

	if (currSkill->mySkill->point > 0) {
		currSkill->state = SkillState::Aiming;
	} 
	else {
		currSkill = nullptr;
		return false;
	}

	currSkill->OnSkill(true);

	return true;
}

ASkill* UPlayerAbilityComp::GetSkill(SkillType type)
{
	ASkill* tempSkill = nullptr;

	switch (type)
	{
	case SkillType::IceAttack: {
		if (IceAttackTime > 0.f)
			return nullptr;
		for (auto i : IceAttack)
			if (i->state == SkillState::Standby) {
				tempSkill = i;
				break;
			}
	}
		 break;

	case SkillType::LightningStrike: {
		if (LightningStrikeTime > 0.f)
			return nullptr;

		for (auto i : LightningStrike)
			if (i->state == SkillState::Standby) {
				tempSkill = i;
				break;
			}
	}
		break;

	case SkillType::Healing: {
		if (HealTime > 0.f)
			return nullptr;

		for (auto i : HealStorm)
			if (i->state == SkillState::Standby) {
				tempSkill = i;
				break;
			}
	}
		break;

	case SkillType::FireStorm: {
		if (FireStormTime > 0.f)
			return nullptr;

		for (auto i : fireStorm)
			if (i->state == SkillState::Standby) {
				tempSkill = i;
				break;
			}
	}
		break;
	}

	return tempSkill;
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

void UPlayerAbilityComp::UpdateValue(FSkillInfo* skillInfo)
{
	if (skillInfo->point <= 0) 
		return;

	if (skillInfo->powerValues.Num() >= skillInfo->point)
		skillInfo->powerValue = skillInfo->powerValues[skillInfo->point - 1];

	if (skillInfo->coolDownValues.Num() >= skillInfo->point)
		skillInfo->coolDownValue = skillInfo->coolDownValues[skillInfo->point - 1];

	if (myPlayer && skillInfo->skillType == SkillType::HpUpgrade) {
		myPlayer->UpgradeHp(skillInfo->powerValue);
	}
	myScreenUI->UnLockSkill(skillInfo->skillType);

}

