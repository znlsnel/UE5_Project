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
	{
		// 스켈레탈 메시 컴포넌트 등록
		pistolMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
		// 부모 컴포넌트 등록
		pistolMeshComp->SetupAttachment(GetMesh());
		// 스켈레탈메시 데이터 로드
		ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/Assets/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
		
		// 데이터 로드가 성공했다면
		if (TempGunMesh.Succeeded())
		{
			// 스켈레탈메시 데이터 할당
			pistolMeshComp->SetSkeletalMesh(TempGunMesh.Object);
			// 위치 조정하기
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

	//  스나이퍼 UI 위젯 인스턴스 생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 일반 조준 크로스헤어 UI
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// 일반 조준 UI 등록
	_crosshairUI->AddToViewport();

	// 권총 들기
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
	// 플레이어 이동
	// GetControlRotation - 플레이어 폰을 컨트롤하고 있는 컨트롤러의 방향을 FRotator 타입으로 넘겨줌
	// FTransform으로 Transform 인스턴트를 생성
	// TrasnformVector 는 특정한 vector를 local vector로 변환시켜줌
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
		// LineTrace의 시작 위치
		FVector startPos = tpsCamComp->GetComponentLocation(); // 카메라의 월드좌표
		//LineTrace의 종료 위치
		FVector endPos = startPos + tpsCamComp->GetForwardVector() * 5000;

		// LineTrace의 충돌 정보를 담을 변수
		FHitResult hitInfo;

		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;

		// 자기 자신은 충돌에서 제외
		params.AddIgnoredActor(this);

		// Channel 필터를 이용한 LineTrace 충돌 검출 (충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션) 
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		// LineTrace가 부딪혔을때
		if (bHit)
		{
			// 충돌 파편 효과 트랜스폼
			FTransform bulletTrans;
			// 부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			// 컴포넌트가 있고 물리가 적용되어있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 날려버릴 힘과 방향이 필요
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;
				// 그 방향으로 날리기
				hitComp->AddForce(force);
			}

			// 부딪힌 대상이 적인지 판단하기
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
	// 권총으로 변경
	bUsingPistolGun = true;
	pistolMeshComp->SetVisibility(true);
	ripleMeshComp->SetVisibility(false);
}

void ATPSPlayer::GetRiple()
{
	// 소총으로 변경
	bUsingPistolGun = false;
	pistolMeshComp->SetVisibility(false);
	ripleMeshComp->SetVisibility(true);
}

void ATPSPlayer::SniperAim()
{
	if (bUsingPistolGun == true) return;

	// Pressed 입력 처리
	if (bSniperAim == false)
	{
		// 스나이퍼 조준 모드 활성화!
		bSniperAim = true;
		// 스나이퍼 조준 UI 등록
		_sniperUI->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
		_crosshairUI->RemoveFromParent();
	}
	// Released 입력 처리
	else
	{
		// 스나이퍼 조준 모드 비활성화!
		bSniperAim = false;
		// 스나이퍼 조준 UI 등록
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
		_crosshairUI->AddToViewport();
	}
}



