// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;

	// MULTICAST
	UFUNCTION(Server, Reliable)
		void PlayMontageInServer(class UAnimMontage* AM);
		void PlayMontageInServer_Implementation(class UAnimMontage* AM);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAnimMontage(class UAnimMontage* AM);
		void MulticastAnimMontage_Implementation(class UAnimMontage* AM);

	UFUNCTION(Server, Reliable)
			void createNiagara(FHitResult pHitResult);
		void createNiagara_Implementation(FHitResult pHitResult);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastNiaga(FHitResult pHitResult);
	void MulticastNiaga_Implementation(FHitResult pHitResult);

	UFUNCTION(Server, Reliable)
		void DoubleClickInServer(DashType dashType);
	void DoubleClickInServer_Implementation(DashType dashType);

	UFUNCTION(NetMulticast, Reliable)
		void DoubleClickMulticast(DashType dashType);
	void DoubleClickMulticast_Implementation(DashType dashType);

public:
	class APlayerController* myController;
	FTimerHandle TickIdTimerHandle;
// COMP=====================================================================
	// 스프링암 Comp
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;		// Spring Arm

	// 카메라 Comp
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;			// Camera

	// PlayerMove
	UPROPERTY(VisibleAnywhere, Category = Component)
		class UPlayerBaseComponent* playerMove;

	// PlayerFire
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
		class UPlayerFire* playerFire;

	UPROPERTY(VisibleAnywhere, blueprintReadOnly, Category = IkFoot)
		class UFootIkActorComponent* IKFootComp;

	UPROPERTY(VisibleAnywhere)
		class UPickupManager* pickupManager;

	UPROPERTY(VisibleAnywhere )
		class UPlayerUI* playerUI;

//======================================================================

// Detail =====================================================================
	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int32 hp;

	// 초기 hp값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int32 initialHp = 100;

	// 피격 달했을 때 처리
	UFUNCTION(BlueprintCallable, Category = Health)
		void OnHitEvent(int damage);

// ===========================================================================
// 
// 변수 ========================================================================

	UPROPERTY(BlueprintReadWrite, Category = weaponOpen)
		bool bRipleOpen = false;

	UPROPERTY(BlueprintReadWrite, Category = weaponOpen)
		bool bPistolOpen = true;

	UPROPERTY(BlueprintReadWrite, Category = Fire)
		FHitResult FireHitResult;

	float AdditionalAttackPower = 0.f;



// Collision 관련 ================================================================

// 무기========================================================================



// U I ========================================================================

	

// 함수========================================================================
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
		void OnGameOver();
	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Health)
	//	void OnUsingPistol(bool isGrenade);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Initialization)
		void OnInitialization();
	//UFUNCTION(BlueprintImplementableEvent, Category = Effect)
	//	void FireEffect();
	class UInventory* GetInventory();
	void GetMoney(int money);
	void tickId();
	//
};
