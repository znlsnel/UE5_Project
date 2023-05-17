// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageUIActor.h"
#include "DamageUI.h"

#include <Components/WidgetComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "TPSPlayer.h"
#include <Camera/PlayerCameraManager.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ADamageUIActor::ADamageUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageUIWidget"));
	

}

// Called when the game starts or when spawned
void ADamageUIActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	//if (player == nullptr)
	//	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Upnunde? ;;"));
	//	
	//
	damageUI = Cast<UDamageUI>(CreateWidget<UDamageUI>(GetWorld(), damageUIFactory));
	widgetComp->SetWidget(damageUI);
}

void ADamageUIActor::AddWorldDamage(float DeltaTime)
{
	AddActorLocalOffset(FVector(0, 0, 50) * DeltaTime);

	addedZPos += 20.f * DeltaTime;

	if (addedZPos > 50)
	{
		Trigger = false;
		SetActorHiddenInGame(true);
		addedZPos = 0.f;
	}
}

void ADamageUIActor::InitializeDamageActor(FVector startPos, FRotator rotator, int Damage, ATPSPlayer* p)
{
	SetActorLocation(startPos);
	SetActorRotation(rotator);
	SetActorHiddenInGame(false);
	Trigger = true;
	damageUI->BindDamageText(Damage);
	myPlayer = p;
}

void ADamageUIActor::SetRot_Implementation()
{
	SetRot_M();
}

void ADamageUIActor::SetRot_M_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer) return;
	if (myPlayer)
	{

		FRotator lookV = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), myPlayer->GetActorLocation());

		SetActorRelativeRotation(lookV);
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Rot : %f, %f, %f"), lookV.Roll, lookV.Yaw, lookV.Pitch));
		
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Player"));

	}
}

// Called every frame
void ADamageUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Trigger)
	{
		AddWorldDamage(DeltaTime);
		SetRot();
	

	}
}

