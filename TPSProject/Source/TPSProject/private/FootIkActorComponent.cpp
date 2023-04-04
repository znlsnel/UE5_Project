// Fill out your copyright notice in the Description page of Project Settings.


#include <Components/CapsuleComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>

#include "FootIkActorComponent.h"
#include "TPSPlayer.h"


// Sets default values for this component's properties
UFootIkActorComponent::UFootIkActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	player = Cast<ATPSPlayer>(GetOwner());
	// ...
}


// Called when the game starts
void UFootIkActorComponent::BeginPlay()
{
	Super::BeginPlay();
	capsuleHalfHeight = player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

}

void UFootIkActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UFootIkActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SkeletonIKTraceInfo leftFootTrace = FootTrace(55.0f, "foot_l_Socket");
	SkeletonIKTraceInfo rightFootTrace = FootTrace(55.0f, "foot_r_Socket");
	
	UpdateFootRotation(DeltaTime, NormalToRotator(leftFootTrace.ImpactLocation), &LeftFootRotation, 13.0f);
	UpdateFootRotation(DeltaTime, NormalToRotator(rightFootTrace.ImpactLocation), &RightFootRotation, 13.0f);
}


struct SkeletonIKTraceInfo UFootIkActorComponent::FootTrace(float TraceDistance, FName socketName)
{
	SkeletonIKTraceInfo TraceInfo;

	FVector socketLocation = player->GetMesh()->GetSocketLocation(socketName);
	FVector startPoint = FVector(socketLocation.X, socketLocation.Y, player->GetActorLocation().Z);
	FVector endPorint = FVector(socketLocation.X, socketLocation.Y,
		player->GetActorLocation().Z - capsuleHalfHeight - TraceDistance);

	FHitResult pHitResult;
	TArray<AActor*> pIgnore;

	bool bDebug = true;
	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bDebug == true) eDebug = EDrawDebugTrace::ForOneFrame;

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), startPoint, endPorint,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, eDebug, pHitResult, true);

	//! Set ImpactNormal and Offset from HitResult
	TraceInfo.ImpactLocation = pHitResult.ImpactNormal;
	if (pHitResult.IsValidBlockingHit() == true)
	{
		float fImpactLegth = (pHitResult.ImpactPoint - pHitResult.TraceEnd).Size();
		TraceInfo.Offset = 5.0f + (fImpactLegth - TraceDistance);
	}
	else
	{
		TraceInfo.Offset = 0.0f;
	}

	return TraceInfo;
}

//IK_FootTrace�Լ����� ���� ����ġ �� �������� �ٴ� Normal Vector�� �� �� �ִ�. 
FRotator UFootIkActorComponent::NormalToRotator(FVector Vector)
{
	float fAtan2_1 = UKismetMathLibrary::DegAtan2(Vector.Y, Vector.Z);
	float fAtan2_2 = UKismetMathLibrary::DegAtan2(Vector.X, Vector.Z);
	fAtan2_2 *= -1.0f;
	FRotator pResult = FRotator(fAtan2_2, 0.0f, fAtan2_1);

	return pResult;

}


//NormalToRotator���� ���� ���� RInterpTo�Լ��� �̿��� ���� �����ϸ� ���ϰ� �ϴ� �Լ��̴�.
void UFootIkActorComponent::UpdateFootRotation(float DeltaTime, FRotator TargetValue, FRotator* FootRotatorValue, float InterpSpeed)
{
	FRotator pInterpRotator = UKismetMathLibrary::RInterpTo(*FootRotatorValue, TargetValue, DeltaTime, InterpSpeed);
	*FootRotatorValue = pInterpRotator;
}


