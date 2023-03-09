// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "Bullet.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		
	}

	// Spring Arm Comp
	{
		springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	
		// RootComponent �� ���������� ĸ�� �ݸ��� ������Ʈ�� �ǹ���
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

	// Gun Skeletal Mesh Component
	{
		// ���̷�Ż �޽� ������Ʈ ���
		gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
		// �θ� ������Ʈ ���
		gunMeshComp->SetupAttachment(GetMesh());
		// ���̷�Ż�޽� ������ �ε�
		ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/Weapons/Rifle/Mesh/SK_Rifle.SK_Rifle'"));
		
		// ������ �ε尡 �����ߴٸ�
		if (TempGunMesh.Succeeded())
		{
			// ���̷�Ż�޽� ������ �Ҵ�
			gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
			// ��ġ �����ϱ�
			gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
		}
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

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Move();
}

void ATPSPlayer::Move()
{
	// �÷��̾� �̵�
	// GetControlRotation - �÷��̾� ���� ��Ʈ���ϰ� �ִ� ��Ʈ�ѷ��� ������ FRotator Ÿ������ �Ѱ���
	// FTransform���� Transform �ν���Ʈ�� ����
	// TrasnformVector �� Ư���� vector�� local vector�� ��ȯ������
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector(0, 0, 0);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, & ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, & ATPSPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::InputJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputHorizontal(float value)
{
	direction.Y = value;
}

void ATPSPlayer::InputVertical(float value)
{
	direction.X = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}


void ATPSPlayer::InputFire()
{
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("Muzzle"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}


