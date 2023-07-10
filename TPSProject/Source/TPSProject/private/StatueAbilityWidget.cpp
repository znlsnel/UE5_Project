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
	if (parent == nullptr || player == nullptr) {
		return false;
	}

	FStatueAbility* tempAbility = GetAbility(type);
	if (tempAbility && player->Grace < tempAbility->cost) {
		return false;
	}

	if (tempAbility->abilityType == AbilityType::StatueRepair)
	{
		if(parent->Hp >= parent->MaxHp)
			return false;

		int recoverHp = parent->MaxHp / 10;
		parent->Hp += recoverHp;
		if (parent->Hp > parent->MaxHp)
			parent->Hp = parent->MaxHp;
	}
	else if (tempAbility->abilityType == AbilityType::SealRepair) {
		isUpdateRepairRate = true;
		NextRepairRate += 1.f / repairCondition;
		UpdateRepairRate();

		GetWorld()->GetTimerManager().ClearTimer(repairRateEndTimer);
		GetWorld()->GetTimerManager().SetTimer(repairRateEndTimer, FTimerDelegate::CreateLambda(
			[&]() {
				isUpdateRepairRate = false;
			}), 3.f, false);
	}
	else if (tempAbility->abilityType == AbilityType::HPUpgrade) {
		int preHp = parent->MaxHp;
		parent->MaxHp = parent->initHp + (tempAbility->point + 5) * 100;

		parent->Hp += parent->MaxHp - preHp;
	}

	player->Grace -= tempAbility->cost;
	tempAbility->point++;	

	abilityInfo->InitWidget(tempAbility);

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
		statueAbilitys.Empty();
		for (auto ability : arr) {
			statueAbilitys.Add(&ability);
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
