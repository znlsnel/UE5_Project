// Fill out your copyright notice in the Description page of Project Settings.


#include "Doomstone.h"
// Sets default values
ADoomstone::ADoomstone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("DoomStone");
	Tags.Add("EnemysTarget");
}

// Called when the game starts or when spawned
void ADoomstone::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("DoomStone");
	Tags.Add("EnemysTarget");

	Hp = MaxHp;
//	hpWidget->parent = this;
}

// Called every frame
void ADoomstone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#include <Kismet/KismetSystemLibrary.h>

void ADoomstone::OnHitEvent(int Damage)
{
	Hp -= Damage;
	if (Hp < 0) Hp = 0;

	HpPercent = (float)Hp / (float)MaxHp;


	if (Hp == 0) {
		DestoryStatue();
		isDestory = true;
	}

}

