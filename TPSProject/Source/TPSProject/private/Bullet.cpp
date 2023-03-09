// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision Setting
	{
		// 충돌체 등록하기
		collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
		// 충돌프로파일 설정
		collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
		// 충돌체 크기 설정
		collisionComp->SetSphereRadius(13);
		// 루트로 등록
		RootComponent = collisionComp;
	}

	// Body Mesh Setting
	{
		// 외관 컴포넌트 등록하기
		bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bodyMeshComp"));
		// 부모 컴포넌트 지정
		bodyMeshComp->SetupAttachment(collisionComp);
		// 충돌 비활성화
		bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 외관 크기 설정
		bodyMeshComp->SetRelativeScale3D(FVector(0.25f));
	}

	// Projectile Setting
	{
		// 발사체 컴포넌트
		movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
		// movement 컴포넌트가 갱신시킬 컴포넌트 지정
		movementComp->SetUpdatedComponent(collisionComp);
		// 초기 속도
		movementComp->InitialSpeed = 5000;
		// 최대 속도
		movementComp->MaxSpeed = 5000;
		// 반동 여부
		movementComp->bShouldBounce = true;
		//반동 값
		movementComp->Bounciness = 0.3f;
	}

	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

