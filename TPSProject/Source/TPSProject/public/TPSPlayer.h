// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponData.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);
DECLARE_MULTICAST_DELEGATE(FTickUpdateFunctions);
DECLARE_DELEGATE(FInteractionObject);
DECLARE_DELEGATE_OneParam(FItemBuyResultButtonFunction, bool result);

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	FInputBindingDelegate onInputBindingDelegate;
	FTickUpdateFunctions tickUpdateFunctions;
	FInteractionObject InteractionDelegate;
	FItemBuyResultButtonFunction ItemBuyResultDelegate;

protected:
	// Called when the game starts or when spawned
	ATPSPlayer();
	~ATPSPlayer();
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void HpRecoveryLoop();
	FTimerHandle hpRecoveryTimer;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;

	UFUNCTION(BlueprintCallable)
		void SetPlayerMouse(bool Active);

	UFUNCTION(BlueprintCallable)
		void OpenStatueAbilityWidget();

	UFUNCTION(BlueprintCallable)
		void StartSequenceBegin();
	UFUNCTION(BlueprintCallable)
		void StartSequenceEnd();

	void StartGame();
	void GetMineralGrace(int mineral, int grace);
	void UpgradeHp(int addHp);
	void GameOver();
	void UpdateHeartSound();
#pragma region ServerFunctions


	// MULTICAST
		// 피격 달했을 때 처리
	UFUNCTION(BlueprintCallable, Category = Health)
		void OnHitEvent(int damage, FVector enemyPos);

	void PlayMontage(class UAnimMontage* AM, FName section = "");
	bool IsPlayingMontage(class UAnimMontage* AM);

	void AddItemInServer(class AItem* item);
	void UpdateAttackAndHp(bool updateAttack, float value);
	
	class UStoreUI* GetStore();

	FTimerHandle SetTranceformBuildableTimer;
#pragma endregion

	static int playerId;
	UFUNCTION(BlueprintPure)
		const int GetPlayerId();


	void InteractionObject();
	void AbilityWidget();
	class UDamageWidget* GetDamageWidget();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UAbilityUpgradeWidget> abilityWidgetFactory;
	UPROPERTY()
		class UAbilityUpgradeWidget* myAbilityWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UDamageWidget> damageWidgetFactory;
	UPROPERTY()
		TArray<class UDamageWidget*> damageWidgets;

	int damageWidgetCount = 30;
	int damageWidgetIndex = 0;
	float lastHitTime = 0.f;
public:
	class ATPSPlayerController* myController;
	FTimerHandle TickIdTimerHandle;
	FTimerHandle PlayerRespawnTimer;
// 
	// 스프링암 Comp
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;		// Spring Arm

	// 카메라 Comp
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;			// Camera

	UPROPERTY(EditAnywhere)
		class UBoxComponent* meleeAttackSensor;

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

	UPROPERTY(EditAnywhere)
		class UItemFactoryComp* itemFactory;

	UPROPERTY(EditAnywhere, blueprintReadOnly)
		class UPlayerAbilityComp* abilityComp;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AM_damage;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AM_Spawn;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* SpawnEffect;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* DamageEffect;



//======================================================================

// Detail =====================================================================
	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int hp;

	// 초기 hp값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		int maxHp = 100;
	int initHp = 100;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DieAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class USoundBase* heartSound;

	UPROPERTY()
		class UAudioComponent* heartAudio;
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

	bool isMovable = true;
	bool isRotatable = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Grace = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Mineral = 0;

	int respawnTime = 10;
	UPROPERTY(Replicated)
	class ABuildableItem* buildableItem;
	TArray<class ABuildableItem*> ItemArr;

	UPROPERTY(EditAnywhere)
		TSubclassOf <class UBuildableItemCheckUI> CheckUIFactory;
	UPROPERTY()
		class UBuildableItemCheckUI* BuildableItemCheckUI;


// 함수========================================================================

	UFUNCTION()
		void BuyItem(EItemID itemId, int ItemGrace, int ItemMineral, int32 ItemCount);
	void CreateItem(TArray<ABuildableItem*>& items, EItemID itemId, int count = 1);
	bool isBuy = false;
	void OnPlayerDie();

	UFUNCTION(BlueprintImplementableEvent, Category = Initialization)
		void OnInitialization();
	FTimerHandle addItemTimer;
	class UInventory* GetInventory();
	void AddHP(int value);

	void SetUnableMove(float AllowMoveTime);
	void SetUnableRotate(float AllowRotateTime);
	FTimerHandle movableTimer;
	FTimerHandle rotatableTimer;
	float allowMoveSecondTime = 0.f;
	float allowRotateSecondTime = 0.f;
};
