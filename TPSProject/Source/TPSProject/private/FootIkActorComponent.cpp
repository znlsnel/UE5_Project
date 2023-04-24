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

	// �� ���� ��ġ�� ������
	FVector socketLocation = player->GetMesh()->GetSocketLocation(socketName);
	
	// LineTrace�� StartPoint = ���� x,y ��ǥ + ĳ������ �߰� ����
	FVector startPoint = FVector(socketLocation.X, socketLocation.Y, player->GetActorLocation().Z);
	// LineTrace�� endPoint = ���� x,y��ǥ + (ĳ������ �߹ٴ� ���� - TraceDistance)��ŭ�� ����
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


//IK_FootTrace�Լ����� ���� ����ġ �� �������� �ٴ� Normal Vector�� �� �� �ִ�. 
FRotator UFootIkActorComponent::NormalToRotator(FVector Vector)
{
	// DegAtan2�� �� ���� �� ������ ��ź��Ʈ�� ����ϰ� ����� �� ������ ��ȯ����
	// vector�� Y�� Z�� Roll�� ���
	float fAtan2_1 = UKismetMathLibrary::DegAtan2(Vector.Y, Vector.Z);
	// vector�� X�� Z�� Pitch�� ���
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



