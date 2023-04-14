// Fill out your copyright notice in the Description page of Project Settings.

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include <Blueprint/UserWidget.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/SkeletalMeshComponent.h>
#include <Blueprint/UserWidget.h>

#include "ScreenUI.h"
#include "Weapon_Pistol.h"
#include "Weapon_Rifle.h"
#include "Weapon_Shotgun.h"
#include "FootIkActorComponent.h"
#include "Gun.h"
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "TPSPlayer.h"



// Sets default values
ATPSPlayer::ATPSPlayer()
{
	//SubObjects
	{
		playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
		playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
		IKFootComp = CreateDefaultSubobject<UFootIkActorComponent>(TEXT("IKFootComp"));
	}



	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Assets/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

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

		// RootComponent 는 계층구조상 캡슐 콜리전 컴포넌트를 의미함
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->SetRelativeLocation(FVector(0, 70, 70));
		springArmComp->TargetArmLength = 400;
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

	// Gun Skeletal Mesh Component
	// 스켈레탈 메시 컴포넌트 등록
	pistolMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// 부모 컴포넌트 등록
	pistolMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));

	// 데이터 로드가 성공했다면
	if (TempGunMesh.Succeeded())
	{
		// 스켈레탈메시 데이터 할당
		pistolMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		pistolMeshComp->SetRelativeLocation(FVector(-7.98, 3.21, -0.89));
		pistolMeshComp->SetRelativeRotation(FRotator(0, 110, 0));
	}
		
	// Sniper Gun Component
	ripleMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	ripleMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("StaticMesh'/Game/Assets/Weapons/Rifle/Mesh/SM_Rifle.SM_Rifle'"));
	if (TempSniperGun.Succeeded())
	{
		ripleMeshComp->SetStaticMesh(TempSniperGun.Object);
		ripleMeshComp->SetRelativeLocation(FVector(-25.74, 0.7, 5.76));
		ripleMeshComp->SetRelativeRotation(FRotator(0, 110, 10));

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

	screenUI = Cast<UScreenUI>(CreateWidget(GetWorld(), ScreenUIFactory));
	screenUI->Initialization(this);

	pistol = GetWorld()->SpawnActor<AWeapon_Pistol>(myPistol);
	pistol->Initialization(this);

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

	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Pressed, this, &ATPSPlayer::PickUp);
	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Released, this, &ATPSPlayer::PickDown);

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



