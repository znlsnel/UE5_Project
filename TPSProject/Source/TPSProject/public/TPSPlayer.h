// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);
DECLARE_MULTICAST_DELEGATE(FTickUpdateFunctions);

DECLARE_DELEGATE(FInteractionObject);

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	FInputBindingDelegate onInputBindingDelegate;
	FTickUpdateFunctions tickUpdateFunctions;
	FInteractionObject InteractionDelegate;

protected:
	// Called when the game starts or when spawned
	ATPSPlayer();
	~ATPSPlayer();
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;
	UFUNCTION(BlueprintCallable)
		void SetPlayerMouse(bool Active);

#pragma region ServerFunctions


	// MULTICAST
		// 피격 달했을 때 처리
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Health)
		void OnHitEvent(int damage);
	void OnHitEvent_Implementation(int damage);

	UFUNCTION(Server, Reliable)
		void OnDamage(int damage);
	void OnDamage_Implementation(int damage);

	UFUNCTION(NetMulticast, Reliable)
		void OnDamageMulti(int damage);
	void OnDamageMulti_Implementation(int damage);

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

	UFUNCTION(Server, Reliable)
		void AddItemInServer(class AItem* item);
	void AddItemInServer_Implementation(class  AItem* item);

	UFUNCTION(NetMulticast, Reliable)
		void AddItemMulticast(class  AItem* item);
	void AddItemMulticast_Implementation(class  AItem* item);


	UFUNCTION(Server, Reliable)
		void DropItemInServer(class AItem* item);
	void DropItemInServer_Implementation(class AItem* item);

	UFUNCTION(NetMulticast, Reliable)
		void DropItemMulticast(class AItem* item);
	void DropItemMulticast_Implementation(class AItem* item);

	UFUNCTION(Server, Reliable)
		void SyncWeaponInServer(class AWeapon* weapon);
		void SyncWeaponInServer_Implementation(class AWeapon* weapon);

	UFUNCTION(NetMulticast, Reliable)
		void SyncWeaponMulticast(class AWeapon* weapon);
		void SyncWeaponMulticast_Implementation(class AWeapon* weapon);

	UFUNCTION(Server, Reliable)
		void ClickWidget(bool isFire);
		void ClickWidget_Implementation(bool isFire);
	UFUNCTION(NetMulticast, Reliable)
		void ClickWidgetMulti(bool isFire);
		void ClickWidgetMulti_Implementation(bool isFire);
	
		UFUNCTION(Server, Reliable)
			void UpdateAttackAndHp(bool updateAttack, float value);
			void UpdateAttackAndHp_Implementation(bool updateAttack, float value);

			UFUNCTION(NetMulticast, Reliable)
				void UpdateAttackAndHpMT(bool updateAttack, float value);
				void UpdateAttackAndHpMT_Implementation(bool updateAttack, float value);

	UFUNCTION(BlueprintImplementableEvent)
		void ClickBPWidget(bool isPressed);

	UFUNCTION(BlueprintImplementableEvent)
		void InitWidgetInteraction();
	class UStoreUI* GetStore();
#pragma endregion

	static int playerId;
	UFUNCTION(BlueprintPure)
		const int GetPlayerId();


	void InteractionObject();

public:
	class ATPSPlayerController* myController;
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
	class UPlayerAnim* playerAnim;

//======================================================================

// Detail =====================================================================
	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int32 hp;

	// 초기 hp값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int32 initialHp = 100;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DieAnimMontage;


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
	UPROPERTY(BlueprintReadOnly)
		bool isDie = false;
	UPROPERTY(BlueprintReadWrite)
		bool isMyPlayer = false;

// 함수========================================================================

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
		void OnGameOver();
		void OnGameOver_Implementation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Initialization)
		void OnInitialization();

	class UInventory* GetInventory();
	void GetMoney(int money);

	//
};
