// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "Bullet.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "EnemyFSM.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Assets/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

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

	// Animation Mode
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
	// Gun Skeletal Mesh Component
	{
		// ���̷�Ż �޽� ������Ʈ ���
		pistolMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
		// �θ� ������Ʈ ���
		pistolMeshComp->SetupAttachment(GetMesh());
		// ���̷�Ż�޽� ������ �ε�
		ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
		
		// ������ �ε尡 �����ߴٸ�
		if (TempGunMesh.Succeeded())
		{
			// ���̷�Ż�޽� ������ �Ҵ�
			pistolMeshComp->SetSkeletalMesh(TempGunMesh.Object);
			// ��ġ �����ϱ�
			pistolMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
		}

		// Sniper Gun Component
		ripleMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
		ripleMeshComp->SetupAttachment(GetMesh());

		ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("StaticMesh'/Game/Assets/Weapons/Rifle/Mesh/SM_Rifle.SM_Rifle'"));
		if (TempSniperGun.Succeeded())
		{
			ripleMeshComp->SetStaticMesh(TempSniperGun.Object);
			ripleMeshComp->SetRelativeLocation(FVector(-22, 55, 120));
			
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

	//  �������� UI ���� �ν��Ͻ� ����
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// �Ϲ� ���� ũ�ν���� UI
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// �Ϲ� ���� UI ���
	_crosshairUI->AddToViewport();

	// ���� ���
	GetPistol();
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
	PlayerInputComponent->BindAction(TEXT("GetPistol"), IE_Pressed, this, &ATPSPlayer::GetPistol);
	PlayerInputComponent->BindAction(TEXT("GetRiple"), IE_Pressed, this, &ATPSPlayer::GetRiple);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Released, this, &ATPSPlayer::SniperAim);

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
	if (bUsingPistolGun)
	{
		FTransform firePosition = pistolMeshComp->GetSocketTransform(TEXT("Muzzle"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else if (bSniperAim)
	{
		// LineTrace�� ���� ��ġ
		FVector startPos = tpsCamComp->GetComponentLocation(); // ī�޶��� ������ǥ
		//LineTrace�� ���� ��ġ
		FVector endPos = startPos + tpsCamComp->GetForwardVector() * 5000;

		// LineTrace�� �浹 ������ ���� ����
		FHitResult hitInfo;

		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;

		// �ڱ� �ڽ��� �浹���� ����
		params.AddIgnoredActor(this);

		// Channel ���͸� �̿��� LineTrace �浹 ���� (�浹 ����, ���� ��ġ, ���� ��ġ, ���� ä��, �浹 �ɼ�) 
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		// LineTrace�� �ε�������
		if (bHit)
		{
			// �浹 ���� ȿ�� Ʈ������
			FTransform bulletTrans;
			// �ε��� ��ġ �Ҵ�
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// �Ѿ� ���� ȿ�� �ν��Ͻ� ����
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			// ������Ʈ�� �ְ� ������ ����Ǿ��ִٸ�
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// �������� ���� ������ �ʿ�
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;
				// �� �������� ������
				hitComp->AddForce(force);
			}

			// �ε��� ����� ������ �Ǵ��ϱ�
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}

void ATPSPlayer::GetPistol()
{
	// �������� ����
	bUsingPistolGun = true;
	pistolMeshComp->SetVisibility(true);
	ripleMeshComp->SetVisibility(false);
}

void ATPSPlayer::GetRiple()
{
	// �������� ����
	bUsingPistolGun = false;
	pistolMeshComp->SetVisibility(false);
	ripleMeshComp->SetVisibility(true);
}

void ATPSPlayer::SniperAim()
{
	if (bUsingPistolGun == true) return;

	// Pressed �Է� ó��
	if (bSniperAim == false)
	{
		// �������� ���� ��� Ȱ��ȭ!
		bSniperAim = true;
		// �������� ���� UI ���
		_sniperUI->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
		_crosshairUI->RemoveFromParent();
	}
	// Released �Է� ó��
	else
	{
		// �������� ���� ��� ��Ȱ��ȭ!
		bSniperAim = false;
		// �������� ���� UI ���
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
		_crosshairUI->AddToViewport();
	}
}



