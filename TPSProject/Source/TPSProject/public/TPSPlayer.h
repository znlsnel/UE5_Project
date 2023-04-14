// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);
DECLARE_MULTICAST_DELEGATE(FTickUpdateFunctions);

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	FInputBindingDelegate onInputBindingDelegate;
	FTickUpdateFunctions tickUpdateFunctions;

protected:
	// Called when the game starts or when spawned
	ATPSPlayer();
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input System
	void PickUp();
	void PickDown();

public:
	bool bisPickUpZone = false;
	bool bPickingUp = false;


public:

// COMP=====================================================================
	// 스프링암 Comp
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;		// Spring Arm

	// 카메라 Comp
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;			// Camera

	// 권총 Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = GunMesh)
		class USkeletalMeshComponent* pistolMeshComp;	// Pistol


	// 소총 Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = GunMesh)
		class UStaticMeshComponent* ripleMeshComp;	// Sniper


	// PlayerMove
	UPROPERTY(VisibleAnywhere, Category = Component)
		class UPlayerBaseComponent* playerMove;

	// PlayerFire
	UPROPERTY(VisibleAnywhere, Category = Component)
		class UPlayerBaseComponent* playerFire;

	UPROPERTY(VisibleAnywhere, blueprintReadOnly, Category = IkFoot)
		class UFootIkActorComponent* IKFootComp;
//======================================================================

// Detail =====================================================================
	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int32 hp;

	// 초기 hp값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int32 initialHp = 10;

	// 피격 달했을 때 처리
	UFUNCTION(BlueprintCallable, Category = Health)
		void OnHitEvent();

// ===========================================================================
// 
// 변수 ========================================================================

	UPROPERTY(BlueprintReadWrite, Category = weaponOpen)
		bool bRipleOpen = false;

	UPROPERTY(BlueprintReadWrite, Category = weaponOpen)
		bool bPistolOpen = true;

	UPROPERTY(BlueprintReadWrite, Category = Fire)
		FHitResult FireHitResult;

// Collision 관련 ================================================================
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

// ===========================================================================


// 무기========================================================================
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon_Pistol> myPistol;
	class AWeapon_Pistol* pistol;
//===========================================================================

// U I ========================================================================
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UScreenUI> ScreenUIFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UScreenUI* screenUI;
//===========================================================================

// 함수========================================================================
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
		void OnGameOver();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Health)
		void OnUsingPistol(bool isGrenade);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Initialization)
		void OnInitialization();
	UFUNCTION(BlueprintImplementableEvent, Category = Effect)
		void FireEffect();
};
