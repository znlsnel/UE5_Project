// Fill out your copyright notice in the Description page of Project Settings.


#include "BomberMan.h"
#include "TPSPlayer.h"
#include "Doomstone.h"
#include "BuildableItem.h"
#include "EnemyAnim.h"
#include "EnemyFSM.h"

#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <NiagaraComponent.h>
#include <NiagaraFunctionLibrary.h>

ABomberMan::ABomberMan() : Super()
{
	Tags.Add(TEXT("BomberMan"));
	Sensor = CreateDefaultSubobject<UBoxComponent>(TEXT("BomberSensor"));
	Sensor->SetupAttachment(GetCapsuleComponent());
	Sensor->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

}

void ABomberMan::BeginPlay()
{
	Super::BeginPlay();
	Sensor->OnComponentBeginOverlap.AddDynamic(this, &ABomberMan::TriggerBomber);
}

void ABomberMan::TriggerBomber(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (fsm->isActive == false || fsm->anim->isDead) return;



	if (OtherActor->ActorHasTag("Player")) {
		Cast<ATPSPlayer>(OtherActor)->OnHitEvent(fsm->anim->AttackDamage, GetActorLocation());
		Explosion();
	}

	else if (OtherActor->ActorHasTag("DoomStone")) {

		ADoomstone* temp = Cast<ADoomstone>(OtherActor);
		temp->OnHitEvent(temp->MaxHp / 5);
		Explosion();
	}

	else if (OtherActor->ActorHasTag("BuildableItem")) {
		ABuildableItem* temp = Cast <ABuildableItem>(OtherActor);
		temp->DamageProcess(temp->MaxShield);
		Explosion();
	}


}

void ABomberMan::Explosion()
{
	fsm->OnDamageProcess(fsm->maxHp * 10, nullptr, "");

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionEffect, GetActorLocation(), GetActorRotation());
}
