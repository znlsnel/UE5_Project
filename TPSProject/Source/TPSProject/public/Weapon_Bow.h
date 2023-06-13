// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponData.h"
#include "Weapon_Bow.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API AWeapon_Bow : public AWeapon
{
	GENERATED_BODY()
public:
	AWeapon_Bow();
	virtual void SynchronizeWhitPlayer(class ATPSPlayer* player)override;
	virtual void Attack()override;
	virtual void Reload()override;
	void Attack(bool isPress);

	void InitArrow();
	void SetArrow();
	void ShootArrow(float power);
	void PlayBowAnim(bool DrawBack);
	class AArrow* GetArrow();


public:
	BowState bowState = BowState::TravelMode;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* BowActiveAM;
	class UPlayerAnim* playerAnim;

	FName DrawBow = FName("Draw");
	FName DrawCancel = FName("Cancel");
	FName Fire = FName("Fire");
	FName ReDraw = FName("ReDraw");

	float lastFireTime = 0.f;
	float lastDrawTime = 0.f;
	float ReDrawTime = 0.5f;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AArrow>arrowFactory;

	UPROPERTY(EditDefaultsOnly)
		class UAnimSequence* NomalAnim;

	UPROPERTY(EditDefaultsOnly)
		class UAnimSequence* PullingAnim;

	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* ArrowAM;



	TArray<class AArrow*>arrows;
	AArrow* arrow;
	int32 MaxArrow = 15;


};
