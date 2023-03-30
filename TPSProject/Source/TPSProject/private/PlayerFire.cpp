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

	// ���� ���
	GetPistol();
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UPlayerFire::InputFire()
{
	// UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

	//APlayerController* controller = GetWorld()->GetFirstPlayerController();
	//controller->PlayerCameraManager->StartCameraShake(cameraShake);

	UPlayerAnim* anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	if (anim)
	{
		anim->PlayAttackAnim();
	}

	if (bUsingPistolGun)
	{
		FTransform firePosition = me->pistolMeshComp->GetSocketTransform(TEXT("Muzzle"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
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
}

void UPlayerFire::GetPistol()
{
	// �������� ����
	bUsingPistolGun = true;

	me->pistolMeshComp->SetVisibility(true);
	me->ripleMeshComp->SetVisibility(false);
}

void UPlayerFire::GetRiple()
{
	if (me->bRipleOpen == false) return;

	// �������� ����
	bUsingPistolGun = false;
	me->pistolMeshComp->SetVisibility(false);
	me->ripleMeshComp->SetVisibility(true);
}

void UPlayerFire::SniperAim()
{
	if (bUsingPistolGun == true) return;

	// Pressed �Է� ó��
	if (bSniperAim == false)
	{
		// �������� ���� ��� Ȱ��ȭ!
		bSniperAim = true;
		// �������� ���� UI ���
		_sniperUI->AddToViewport();
		//tpsCamComp->SetFieldOfView(45.0f);
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