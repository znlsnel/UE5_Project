// Fill out your copyright notice in the Description page of Project Settings.


#include "BarricadeItem.h"
#include "Enemy.h"

#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>

ABarricadeItem::ABarricadeItem() : Super()
{
	Tags.Add("Barricade");
	overlapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Sensor"));

	overlapCollision->AttachToComponent(boxCollision, FAttachmentTransformRules::KeepRelativeTransform);
}


void ABarricadeItem::BeginPlay()
{
	Super::BeginPlay();

	// OverlapCollision Init
	overlapCollision->OnComponentBeginOverlap.AddDynamic(this, &ABarricadeItem::BegineOverlapEvent);
	overlapCollision->OnComponentEndOverlap.AddDynamic(this, &ABarricadeItem::EndOverlapEvent);
	FVector boxSize = boxCollision->GetScaledBoxExtent();
	boxSize *= 1.3f;
	overlapCollision->SetBoxExtent(boxSize);
	overlapCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);


}


void ABarricadeItem::BegineOverlapEvent(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy")) {

		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Enemy! Overlap!"));
		AEnemy* tempEnemy = Cast<AEnemy>(OtherActor);
		enemys.Add(tempEnemy, enemyId++);
		tempEnemy->SetTarget(this);
	}
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("! Overlap!"));


}


void ABarricadeItem::EndOverlapEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Enemy") && enemys.IsEmpty() == false) {
		enemys.FindAndRemoveChecked(Cast<AEnemy>(OtherActor));
	}

}

void ABarricadeItem::DamageProcess(int Damage)
{
	Super::DamageProcess(Damage);

	overlapCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
