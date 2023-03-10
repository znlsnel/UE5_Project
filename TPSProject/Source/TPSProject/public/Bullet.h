// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPSPROJECT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:


public:
	// �߻�ü�� �̵��� ����� ������Ʈ
	UPROPERTY (VisibleAnywhere, category = movement)
		class UProjectileMovementComponent* movementComp;

	// �浹 ������Ʈ
	UPROPERTY (VisibleAnywhere, Category = Collision)
		class USphereComponent* collisionComp;

	// �ܰ� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
		class UStaticMeshComponent* bodyMeshComp;

	// �Ѿ� �ӵ�
	UPROPERTY(EditAnywhere, Category = Settings) 
		float speed = 5000;


	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
