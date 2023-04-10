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
	// �Ѿ� ����
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
	//  �������� UI ���� �ν��Ͻ� ����
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// �Ϲ� ���� ũ�ν���� UI
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// �Ϲ� ���� UI ���
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
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), bulletSound, me->GetActorLocation());
		//controller->PlayerCameraManager->StartCameraShake(cameraShake);

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
		me->LineTraceEndPos = LineTraceEnd;
		//GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else if (bSniperAim)
	{
		// LineTrace�� ���� ��ġ
		FVector startPos = me->tpsCamComp->GetComponentLocation(); // ī�޶��� ������ǥ
		
		//LineTrace�� ���� ��ġ
		FVector endPos = startPos + me->tpsCamComp->GetForwardVector() * 5000;

		// LineTrace�� �浹 ������ ���� ����
		FHitResult hitInfo;

		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;

		// �ڱ� �ڽ��� �浹���� ����
		params.AddIgnoredActor(me);

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

	me->FireEffect();
}

void UPlayerFire::GetPistol()
{
	// �������� ����
	bUsingPistol = true;

	me->pistolMeshComp->SetVisibility(true);
	me->ripleMeshComp->SetVisibility(false);

	me->OnUsingPistol(bUsingPistol);
}

void UPlayerFire::GetRiple()
{
	if (me->bRipleOpen == false) return;

	// �������� ����
	bUsingPistol = false;
	me->pistolMeshComp->SetVisibility(false);
	me->ripleMeshComp->SetVisibility(true);

	me->OnUsingPistol(bUsingPistol);
}

void UPlayerFire::SniperAim()
{
	if (bUsingPistol == true) return;

	// Pressed �Է� ó��
	if (bSniperAim == false)
	{
		// �������� ���� ��� Ȱ��ȭ!
		bSniperAim = true;
		// �������� ���� UI ���
		_sniperUI->AddToViewport();
		me->tpsCamComp->SetFieldOfView(45.0f);
		_crosshairUI->RemoveFromParent();
	}
	// Released �Է� ó��
	else
	{
		// �������� ���� ��� ��Ȱ��ȭ!
		bSniperAim = false;
		// �������� ���� UI ���
		_sniperUI->RemoveFromParent();
		me->tpsCamComp->SetFieldOfView(90.0f);
		_crosshairUI->AddToViewport();
	}
}