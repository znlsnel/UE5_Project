// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootIkActorComponent.generated.h"

typedef struct SkeletonIKTraceInfo
{
	float Offset;
	FVector ImpactLocation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UFootIkActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:


public:	
	// Sets default values for this component's properties
	UFootIkActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	struct SkeletonIKTraceInfo FootTrace(float TraceDistance, FName SocketName);
	FRotator NormalToRotator(FVector Vector);
	void UpdateFootRotation(float DeltaTime, FRotator TargetValue, FRotator* FootRotatorValue, float InterpSpeed);



public:
	float capsuleHalfHeight;
	class ATPSPlayer* player;

	UPROPERTY(blueprintReadOnly, category = FootIK)
		FRotator LeftFootRotation;
	UPROPERTY(blueprintReadOnly, category = FootIK)
		FRotator RightFootRotation;
};
