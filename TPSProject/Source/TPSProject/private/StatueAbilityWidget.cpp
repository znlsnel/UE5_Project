// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueAbilityWidget.h"
#include "StatueAbilityInfoWidget.h"
#include "Doomstone.h"
#include "TPSPlayer.h"

#include <Kismet/GameplayStatics.h>

void UStatueAbilityWidget::NativeConstruct()
{
	parent = Cast<ADoomstone>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoomstone::StaticClass()));
	player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	abilityInfo = CreateWidget<UStatueAbilityInfoWidget>(GetWorld(), statueAblityInfoFactory);
	LoadJson();
}

void UStatueAbilityWidget::LoadJson()
{
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectContentDir() + "\\Assets\\Json\\JS_StatueAbility.json"));
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> JsonValueArray = JsonObject->GetArrayField(TEXT("StatueAbility"));

		for (auto jsonValue : JsonValueArray) {
			TSharedPtr<FJsonObject> jsonValueObject = jsonValue->AsObject();

			FStatueAbility* tempAbility = new FStatueAbility();
			tempAbility->abilityName = jsonValueObject->GetStringField(TEXT("abilityName"));
			tempAbility->abilityInfo = jsonValueObject->GetStringField(TEXT("abilityInfo"));
			tempAbility->abilityType = ConvertAbilityType(jsonValueObject->GetStringField(TEXT("abilityType")));
			tempAbility->cost = jsonValueObject->GetIntegerField(TEXT("cost"));

			statueAbilitys.Add(tempAbility);
		}
	}
}

bool UStatueAbilityWidget::UpgradeAbility(AbilityType type)
{
	FStatueAbility* ability = GetAbility(type);

	bool nullCheck = parent == nullptr || player == nullptr || ability == nullptr;
	bool costCheck = player->Grace < ability->cost;

	if (nullCheck || costCheck)
		return false;
	
	if  (UpgradeAbility(ability)) {
		player->Grace -= ability->cost;
		ability->point++;

		abilityInfo->InitWidget(ability);
		return true;
	}

	return false;
}

bool UStatueAbilityWidget::UpgradeAbility(FStatueAbility* ability)
{
	if (ability->abilityType == AbilityType::StatueRepair)
	{
		if (parent->Hp >= parent->MaxHp)
			return false;

		int recoverHp = parent->MaxHp / 10;
		parent->Hp += recoverHp;
		if (parent->Hp > parent->MaxHp)
			parent->Hp = parent->MaxHp;
	}
	else if (ability->abilityType == AbilityType::SealRepair) {
		
		isUpdateRepairRate = true;
		NextRepairRate = (1.f * ability->point) / repairCondition;
		UpdateRepairRate();

		GetWorld()->GetTimerManager().ClearTimer(repairRateEndTimer);
		GetWorld()->GetTimerManager().SetTimer(repairRateEndTimer, FTimerDelegate::CreateLambda(
			[&]() {
				isUpdateRepairRate = false;
			}), 3.f, false);
	}
	else if (ability->abilityType == AbilityType::HPUpgrade) {
		int preHp = parent->MaxHp;
		parent->MaxHp = parent->initHp + (ability->point * 5) * 100;

		parent->Hp += parent->MaxHp - preHp;
	}
	return true;
}

void UStatueAbilityWidget::HorverButton(bool isHorver, AbilityType type, FVector2D pos)
{
	if (abilityInfo->IsInViewport() == false)
		abilityInfo->AddToViewport();
	if (isHorver == false) {
		abilityInfo->CloseWidget();
		return;
	}
	abilityInfo->SetPositionInViewport(pos);
	FStatueAbility* tempAbility = GetAbility(type);

	if (tempAbility == nullptr) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Ability"));
		return;
	}
	if (abilityInfo == nullptr) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No abilityInfo"));
		return;
	}

	abilityInfo->InitWidget(tempAbility);
	abilityInfo->OpenWidget();
}

void UStatueAbilityWidget::UpdateRepairRate()
{
	GetWorld()->GetTimerManager().ClearTimer(repairRateLoopTimer);

	if (FMath::Abs(NextRepairRate - repairRate) < 0.003f || isUpdateRepairRate == false) {
		repairRate = NextRepairRate;
		return;
	}
	repairRate = FMath::FInterpTo(repairRate, NextRepairRate, 0.1f, 3.f);

	GetWorld()->GetTimerManager().SetTimer(repairRateLoopTimer, this, &UStatueAbilityWidget::UpdateRepairRate, 0.1f, false);
}

void UStatueAbilityWidget::SetGetStatueAbilityArr(TArray<FStatueAbility>& arr, float& m_repairRate, bool Set)
{
	if (Set) {
		for (auto ability : arr) {
			FStatueAbility* temp = GetAbility(ability.abilityType);
			if (temp) {
				temp->cost = ability.cost;
				UpgradeAbility(temp);
			}
		}
		repairRate = m_repairRate;
		return;
	}
	arr.Empty();
	for (auto ability : statueAbilitys) {
		arr.Add(*ability);
	}
	m_repairRate = repairRate;
}


void UStatueAbilityWidget::CloseWidget()
{
	abilityInfo->CloseWidget();
}

AbilityType UStatueAbilityWidget::ConvertAbilityType(FString type)
{
	if (type == "SealRepair")
		return AbilityType::SealRepair;
	else if (type == "StatueRepair")
		return AbilityType::StatueRepair;
	else
		return AbilityType::HPUpgrade;
}

FStatueAbility* UStatueAbilityWidget::GetAbility(AbilityType type)
{
	for (auto ability : statueAbilitys) {
		if (ability->abilityType == type)
			return ability;
	}
	return nullptr;
}
