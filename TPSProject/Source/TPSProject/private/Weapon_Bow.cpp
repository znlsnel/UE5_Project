// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Bow.h"
#include "Weapon_BowAnim.h"
#include "PlayerAnim.h"
#include "Arrow.h"

#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Camera/CameraComponent.h>

AWeapon_Bow::AWeapon_Bow()
{

	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weaponmeshcomp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/BowsAndCrossbowsPack/Assets/Models/LongBow_01/SK_LongBow_01.SK_LongBow_01'"));

	if (tempMesh.Succeeded())
	{
		weaponMeshComp->SkeletalMesh = tempMesh.Object;
	}

	RootComponent = weaponMeshComp;

	pickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("pickupCollision"));

	pickupCollision->SetRelativeLocation(FVector(0, 0, 10));
	pickupCollision->SetRelativeScale3D(FVector(1, 1, 2.25));
	pickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	pickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	pickupCollision->SetupAttachment(RootComponent);

	weaponType = WeaponType::Bow;
	attachCharacterSocketName = FName("LeftHandSocket");
	ammoType = AmmoType::None;
	weaponSlotType = WeaponSlotType::None;
	fireDelay = 0.5f;
}

void AWeapon_Bow::SynchronizeWhitPlayer(ATPSPlayer* player)
{
	Super::SynchronizeWhitPlayer(player);
	SetActorRelativeLocation(FVector(5.688160, -1.862145, 3.300226));
	SetActorRelativeRotation(FRotator(-6.309163, -105.981231, -0.805839));
	playerAnim = myPlayer->playerAnim;

	InitArrow();
}

void AWeapon_Bow::Attack()
{

}

void AWeapon_Bow::Attack(bool isPress)
{
	FName section = "";
	if (isPress)
	{
		lastDrawTime = GetWorld()->GetTimeSeconds();
		if (lastDrawTime - lastFireTime < ReDrawTime)
			section = ReDraw;
		else
			section =  DrawBow;
		myPlayer->PlayBowAnim(true);
		myPlayer->SetArrow();
		
	}
	else
	{
		lastFireTime = GetWorld()->GetTimeSeconds();

		float pow = FMath::Min(lastFireTime - lastDrawTime, 1.5f);

		if (pow > 0.4f)
			section = Fire;
		else
			section = DrawCancel;
		myPlayer->PlayBowAnim(false);
		myPlayer->ShootArrow(pow);
	}
	myPlayer->PlayMontageInServer(BowActiveAM, section);
	
}



void AWeapon_Bow::InitArrow()
{
	for (int i = 0; i < MaxArrow; i++)
	{
		AArrow* tempArrow = Cast<AArrow>(GetWorld()->SpawnActor(arrowFactory));

		if (IsValid(tempArrow))
			arrows.Add(tempArrow);
	}
}

void AWeapon_Bow::SetArrow()
{
	FVector pos = myPlayer->GetMesh()->GetSocketLocation("RightHandSocket");
	pos = weaponMeshComp->GetSocketLocation("ArrowTailSocket");
	arrow = GetArrow();
	if (IsValid(arrow) == false)  
		return;

	arrow->SetActorLocation(pos);
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("pos : %f, %f, %f"), pos.X, pos.Y, pos.Z ));

	arrow->AttachToComponent(weaponMeshComp, FAttachmentTransformRules::KeepWorldTransform, "ArrowTailSocket");


	
	arrow->InitArrow(weaponMeshComp->GetSocketLocation("ArrowHeadSocket"));
}


void AWeapon_Bow::ShootArrow(float power)
{

	if (IsValid(arrow) == false) return;

	FVector TraceStartPoint;
	FRotator TraceStartRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, OUT TraceStartRotation);

	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector ShootDir = camLocation.ForwardVector * 10000;

	arrow->ShootArrow(TraceStartPoint + TraceStartRotation.Vector() * 10000, power);
}

void AWeapon_Bow::PlayBowAnim(bool DrawBack)
{
	UWeapon_BowAnim* Anim = Cast<UWeapon_BowAnim>(weaponMeshComp->GetAnimInstance());
	if (DrawBack)
	{
		Anim->bowState = BowState::AimMode;
		//Anim->Montage_JumpToSection("DrawBack", ArrowAM);
	}
	else
	{
		Anim->bowState = BowState::TravelMode;
		Anim->Montage_JumpToSection("Fire", ArrowAM);
	}
	
}

AArrow* AWeapon_Bow::GetArrow()
{
	for (auto arw : arrows)
	{
		if (arw->bIsUsed == false)
			return arw;
	}
	return nullptr;
}
