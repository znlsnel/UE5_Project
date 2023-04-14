// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "EnemyFSM.h"
#include "Bullet.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathVectorCommon.h>

UPlayerFire::UPlayerFire()
{
	// 총알 사운드
	{
		ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/Assets/Sounds/Rifle.Rifle'"));
		if (tempSound.Succeeded())
		{
			bulletSound = tempSound.Object;
		}
	}
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(TEXT("GetPistol"), IE_Pressed, this, &UPlayerFire::GetPistol);
	PlayerInputComponent->BindAction(TEXT("GetRiple"), IE_Pressed, this, &UPlayerFire::GetRiple);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Pressed, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(TEXT("ScopeMode"), IE_Released, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();
	//  스나이퍼 UI 위젯 인스턴스 생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 일반 조준 크로스헤어 UI
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// 일반 조준 UI 등록
	_crosshairUI->AddToViewport();
	controller = GetWorld()->GetFirstPlayerController();

	me->OnInitialization();


	GetPistol();
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UPlayerFire::InputFire()
{
	UPlayerAnim* anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	if (anim)
	{
		anim->PlayAttackAnim();
	}

	if (bUsingPistol)
	{
		
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), bulletSound, me->GetActorLocation());
		//controller->PlayerCameraManager->StartCameraShake(cameraShake);

		//ConstructorHelpers::FObjectFinder<UAnimationAsset> tempAnim(TEXT("AnimSequence'/Game/Assets/Weapons/Pistol/Animations/Weap_Pistol_Fire.Weap_Pistol_Fire'"));
		//if (tempAnim.Succeeded())
		//	me->pistolMeshComp->PlayAnimation(tempAnim.Object, false);

		FVector TraceStartPoint;
		FRotator TraceStartRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT TraceStartPoint, TraceStartRotation);

		float traceLength = 5000.f;
		FVector LineTraceEnd = TraceStartPoint + TraceStartRotation.Vector() * traceLength;

		FHitResult pHitResult;
		TArray<AActor*> pIgnore;
		pIgnore.Add(GetOwner());

		bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStartPoint, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, EDrawDebugTrace::None, pHitResult, true);

		if (isHit)
			LineTraceEnd = pHitResult.ImpactPoint;

		FTransform firePosition = me->pistolMeshComp->GetSocketTransform(TEXT("Muzzle"));
		TraceStartPoint = firePosition.GetLocation();

		//DrawDebugLine(GetWorld(), TraceStartPoint, LineTraceEnd, FColor(255, 0, 0), false, 2.f, 0.f, 10.f);
		me->FireHitResult = pHitResult;
		//GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else if (bSniperAim)
	{
		// LineTrace의 시작 위치
		FVector startPos = me->tpsCamComp->GetComponentLocation(); // 카메라의 월드좌표
		
		//LineTrace의 종료 위치
		FVector endPos = startPos + me->tpsCamComp->GetForwardVector() * 5000;

		// LineTrace의 충돌 정보를 담을 변수
		FHitResult hitInfo;

		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;

		// 자기 자신은 충돌에서 제외
		params.AddIgnoredActor(me);

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

	me->FireEffect();
}

void UPlayerFire::GetPistol()
{
	// 권총으로 변경
	bUsingPistol = true;

	me->pistolMeshComp->SetVisibility(true);
	me->ripleMeshComp->SetVisibility(false);

	me->OnUsingPistol(bUsingPistol);
}

void UPlayerFire::GetRiple()
{
	if (me->bRipleOpen == false) return;

	// 소총으로 변경
	bUsingPistol = false;
	me->pistolMeshComp->SetVisibility(false);
	me->ripleMeshComp->SetVisibility(true);

	me->OnUsingPistol(bUsingPistol);
}

void UPlayerFire::SniperAim()
{
	if (bUsingPistol == true) return;

	// Pressed 입력 처리
	if (bSniperAim == false)
	{
		// 스나이퍼 조준 모드 활성화!
		bSniperAim = true;
		// 스나이퍼 조준 UI 등록
		_sniperUI->AddToViewport();
		me->tpsCamComp->SetFieldOfView(45.0f);
		_crosshairUI->RemoveFromParent();
	}
	// Released 입력 처리
	else
	{
		// 스나이퍼 조준 모드 비활성화!
		bSniperAim = false;
		// 스나이퍼 조준 UI 등록
		_sniperUI->RemoveFromParent();
		me->tpsCamComp->SetFieldOfView(90.0f);
		_crosshairUI->AddToViewport();
	}
}