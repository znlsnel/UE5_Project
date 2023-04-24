// Fill out your copyright notice in the Description page of Project Settings.



#include "FootIkActorComponent.h"
#include "TPSPlayer.h"

#include <Components/CapsuleComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>

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

	SkeletonIKTraceInfo leftFootTrace = FootTrace(50.f, "foot_l");
	SkeletonIKTraceInfo rightFootTrace = FootTrace(50.f, "foot_r");
	
	UpdateFootRotation(DeltaTime, NormalToRotator(leftFootTrace.ImpactLocation), &LeftFootRotation, 13.0f);
	UpdateFootRotation(DeltaTime, NormalToRotator(rightFootTrace.ImpactLocation), &RightFootRotation, 13.0f);

	float tempHipsOffset = UKismetMathLibrary::Min(leftFootTrace.Offset, rightFootTrace.Offset);


	if (tempHipsOffset < 0.0f == false)
		tempHipsOffset = 0.0f;
	
	UpdateFootOffset(DeltaTime, tempHipsOffset, &HipsOffset, 13.0f);
	UpdateFootOffset(DeltaTime, leftFootTrace.Offset - tempHipsOffset, &LeftFootOffset, 13.0f);
	UpdateFootOffset(DeltaTime, -1 * (rightFootTrace.Offset - tempHipsOffset), &RightFootOffset, 13.0f);

	//UpdateCapsuleHalfHeight(DeltaTime, tempHipsOffset, false);
	



}


struct SkeletonIKTraceInfo UFootIkActorComponent::FootTrace(float TraceDistance, FName socketName)
{
	SkeletonIKTraceInfo TraceInfo;

	// 양 발의 위치를 가져옴
	FVector socketLocation = player->GetMesh()->GetSocketLocation(socketName);
	
	// LineTrace의 StartPoint = 발의 x,y 좌표 + 캐릭터의 중간 높이
	FVector startPoint = FVector(socketLocation.X, socketLocation.Y, player->GetActorLocation().Z);
	// LineTrace의 endPoint = 발의 x,y좌표 + (캐릭터의 발바닥 높이 - TraceDistance)만큼의 높이
	FVector endPoint = FVector(socketLocation.X, socketLocation.Y,
		(player->GetActorLocation().Z - capsuleHalfHeight) - TraceDistance);

	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	bool bDebug = true;
	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bDebug == true) eDebug = EDrawDebugTrace::None;


	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), startPoint, endPoint,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, eDebug, pHitResult, true);

	
	TraceInfo.ImpactLocation = pHitResult.Normal;
	if (pHitResult.IsValidBlockingHit())
	{
		float fImpactLegth = (pHitResult.ImpactPoint - pHitResult.TraceEnd).Size();
		TraceInfo.Offset = 3.0f + (fImpactLegth - TraceDistance);
	}
	else
	{
		TraceInfo.Offset = 0.0f;
	}

	return TraceInfo;
}


//IK_FootTrace함수에서 나온 땅위치 값 바탕으로 바닥 Normal Vector를 알 수 있다. 
FRotator UFootIkActorComponent::NormalToRotator(FVector Vector)
{
	// DegAtan2는 두 인자 간 비율의 역탄젠트를 계산하고 결과를 도 단위로 반환해줌
	// vector의 Y와 Z로 Roll을 계산
	float fAtan2_1 = UKismetMathLibrary::DegAtan2(Vector.Y, Vector.Z);
	// vector의 X와 Z로 Pitch를 계산
	float fAtan2_2 = UKismetMathLibrary::DegAtan2(Vector.X, Vector.Z);
	fAtan2_2 *= -1.0f;
	FRotator pResult = FRotator(fAtan2_2, 0.0f, fAtan2_1);

	return pResult;

}


//NormalToRotator에서 나온 값을 RInterpTo함수를 이용해 값이 보간하며 변하게 하는 함수이다.
void UFootIkActorComponent::UpdateFootRotation(float DeltaTime, FRotator TargetValue, FRotator* FootRotatorValue, float InterpSpeed)
{
	FRotator pInterpRotator = UKismetMathLibrary::RInterpTo(*FootRotatorValue, TargetValue, DeltaTime, InterpSpeed);
	*FootRotatorValue = pInterpRotator;
}

void UFootIkActorComponent::UpdateCapsuleHalfHeight(float deltaTime, float hipsShift, bool resetDefault)
{
	UCapsuleComponent* capsuleComp = player->GetCapsuleComponent();

	if (!capsuleComp) return;

	float capsuleHalf = 0.0f;

	if (resetDefault == true)
		capsuleHalf = capsuleHalfHeight;
	else
	{
		float halfAbsSize = UKismetMathLibrary::Abs(hipsShift) * 0.5f;
		capsuleHalf = capsuleHalfHeight - halfAbsSize;
	}

	float fScaledCapsuleHalfHeight = capsuleComp->GetScaledCapsuleHalfHeight();
	float fInterpValue = UKismetMathLibrary::FInterpTo(fScaledCapsuleHalfHeight, capsuleHalf, deltaTime, 13.0f);

	capsuleComp->SetCapsuleHalfHeight(fInterpValue, true);
}

void UFootIkActorComponent::UpdateFootOffset(float DeltaTime, float TargetValue, float* EffectorValue, float InterpSpeed)
{
	float InterpValue = UKismetMathLibrary::FInterpTo(*EffectorValue, TargetValue, DeltaTime, InterpSpeed);
	*EffectorValue = InterpValue;
}



