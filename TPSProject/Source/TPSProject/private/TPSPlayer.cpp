// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include "ScreenUI.h"
#include "Weapon_Pistol.h"
#include "Weapon_Rifle.h"
#include "Weapon_Shotgun.h"
#include "Weapon.h"
#include "FootIkActorComponent.h"
#include "Gun.h"
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "PickupManager.h"
#include "PlayerUI.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include <Blueprint/UserWidget.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/SkeletalMeshComponent.h>
#include <Blueprint/UserWidget.h>




// Sets default values
ATPSPlayer::ATPSPlayer()
{
	//SubObjects
	{
		playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
		playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
		IKFootComp = CreateDefaultSubobject<UFootIkActorComponent>(TEXT("IKFootComp"));
		pickupManager = CreateDefaultSubobject<UPickupManager>(TEXT("PickUpManager"));
		playerUI = CreateDefaultSubobject<UPlayerUI>(TEXT("UI"));
	}



	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.15), FRotator(0, -90, 0));

	}

	// Bind Collision Event
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSPlayer::BeginOverlap);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ATPSPlayer::EndOverlap);
	}
	//OnComponentBeginOverlap.AddDynamic(this, &AGun::BeginOverlap);

// Spring Arm Comp
	{
		springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

		// RootComponent ´Â °èÃþ±¸Á¶»ó Ä¸½¶ ÄÝ¸®Àü ÄÄÆ÷³ÍÆ®¸¦ ÀÇ¹ÌÇÔ
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->SetRelativeLocation(FVector(0, 70, 70));
		springArmComp->TargetArmLength = 250;
		springArmComp->bUsePawnControlRotation = true;
	}

	// TPS Cam Comp
	{
		tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
		tpsCamComp->SetupAttachment(springArmComp);
		tpsCamComp->bUsePawnControlRotation = false;
	}

	// Animation Mode
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}


	// Self
	{
		bUseControllerRotationYaw = true;

		// CharactorMovement
		{
			JumpMaxCount = 2;
		}
	}


}



void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();


	hp = initialHp;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	tickUpdateFunctions.Broadcast();
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	onInputBindingDelegate.Broadcast(PlayerInputComponent);

}









void ATPSPlayer::PickUp()
{
	if (bisPickUpZone == false) return;
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("pickUpZone : true"), true, true, FLinearColor::Black, 2.0f);

	bPickingUp = true;


}

void ATPSPlayer::PickDown()
{
	if (bisPickUpZone == false) return;
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("pickUpZone : false"), true, true, FLinearColor::Black, 2.0f);

	bPickingUp = false;

}





void ATPSPlayer::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGun* tempWidget = Cast<AGun>(OtherActor);
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("pickUpZone : Begin"), true, true, FLinearColor::Black, 2.0f);


	if (tempWidget)
	{
		bisPickUpZone = true;
	}
}

void ATPSPlayer::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGun* tempWidget = Cast<AGun>(OtherActor);
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("pickUpZone : End"), true, true, FLinearColor::Black, 2.0f);

	if (tempWidget)
	{
		bisPickUpZone = false;
		bPickingUp = false;
	}
}

UInventory* ATPSPlayer::GetInventory()
{
	return playerUI->screenUI->inventory;
}






void ATPSPlayer::OnHitEvent()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damaged!!!"));
	hp -= 5;
	if (hp <= 0)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Player is Dead!"));
		OnGameOver();
	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}



