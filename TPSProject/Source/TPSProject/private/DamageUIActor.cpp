// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageUIActor.h"
#include "DamageUI.h"

#include <Components/WidgetComponent.h>
#include <Kismet/KismetSystemLibrary.h>

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

	damageUI = Cast<UDamageUI>(CreateWidget<UDamageUI>(GetWorld(), damageUIFactory));
	widgetComp->SetWidget(damageUI);
}

void ADamageUIActor::AddWorldDamage(float DeltaTime)
{
	AddActorLocalOffset(FVector(0, 0, 20) * DeltaTime);

	addedZPos += 20.f * DeltaTime;

	if (addedZPos > 50)
	{
		Trigger = false;
		SetActorHiddenInGame(true);
		addedZPos = 0.f;
	}
}

void ADamageUIActor::InitializeDamageActor(FVector startPos, FRotator rotator, int Damage)
{
	SetActorLocation(startPos);
	SetActorRotation(rotator);
	SetActorHiddenInGame(false);
	Trigger = true;
	damageUI->BindDamageText(Damage);
}

// Called every frame
void ADamageUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Trigger) AddWorldDamage(DeltaTime);
}

