// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Components/SphereComponent.h>
#include <Blueprint/UserWidget.h>

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//FMath::SRandInit(0);

	// Collision
	{
		acquireCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AcquiredRange"));
		acquireCollision->SetCollisionProfileName(TEXT("OverlapAll"));
		acquireCollision->SetSphereRadius(150.f);
		acquireCollision->OnComponentBeginOverlap.AddDynamic(this, &AGun::BeginOverlap);
		acquireCollision->OnComponentEndOverlap.AddDynamic(this, &AGun::EndOverlap);
		RootComponent = acquireCollision;
	}

	//Mesh
	{
		bodyMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));

		ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Rifle/Mesh/SK_Rifle.SK_Rifle'"));

		if (tempMesh.Succeeded())
		{
			bodyMeshComp->SetSkeletalMesh(tempMesh.Object);
			bodyMeshComp->SetRelativeRotation(FRotator(0, 0, 0));
			bodyMeshComp->SetupAttachment(acquireCollision);
		}
	}


	

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PickUpCheck();
}

void AGun::PickUpCheck()
{
	if (player)
	{
		if (player->bisPickUpZone == false)
		{
			if (pickUpWD && pickUpWD->IsInViewport())
				pickUpWD->RemoveFromViewport();
			return;
		}

		if (!pickUpWD)
			pickUpWD = CreateWidget(GetWorld(), Ts_PickUpWD);

		if (player->bPickingUp && pickUpWD->IsInViewport() == false)
		{
			pickUpWD->AddToViewport();
			
		}

		if (player->bPickingUp == false && pickUpWD->IsInViewport())
		{
			pickUpWD->RemoveFromViewport();
		}
	}
}

void AGun::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<ATPSPlayer>(OtherActor);
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Begin"), true, true, FLinearColor::Black, 2.0f);

	if (player)
	{


	}
	
}

void AGun::EndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("End"), true, true, FLinearColor::Black, 2.0f);

	//if (pickUpWD->IsInViewport())
	//	pickUpWD->RemoveFromViewport();
}


