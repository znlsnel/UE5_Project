// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerAbilityComp.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill.generated.h"

UENUM(BlueprintType)
enum class SkillState : uint8
{
	Aiming,
	Operating,
	Standby,
};
UCLASS()
class TPSPROJECT_API ASkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkill();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnSkill(bool useSkill);

	void TriggerSkill();
	void SetLocatoin();
	void OnEffect(bool isUsed);
	FHitResult LineTrace();
	FHitResult hitResult;
	TArray<AActor*> enemySensorArray;
public:
	UPROPERTY(EditAnywhere)
		class USphereComponent* enemySensor;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* SkillEffect;

	UPROPERTY(EditAnywhere)
		class UDecalComponent* SkillPreview;

	UPROPERTY(EditAnywhere)
		class UPointLightComponent* Light;

	UPROPERTY(EditAnywhere)
		SkillType skillType = SkillType::None;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* previewLine;

	UPROPERTY(EditAnywhere)
		USoundBase* skillSound;

	SkillState state = SkillState::Standby;
	FTimerHandle SkillLoopTimer;
	FSkillInfo* mySkill;
	class ATPSPlayer* myPlayer;
	class APlayerController* myPlayerControler;

	// 조준중 -> 작동중 -> 미작동중

	float beginLoopTime = 0.f;
	float skillDurationTime = 0.f;


};
