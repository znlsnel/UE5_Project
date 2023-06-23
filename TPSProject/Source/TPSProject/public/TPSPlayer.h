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
	void HpRecoveryLoop();
	FTimerHandle hpRecoveryTimer;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;
	UFUNCTION(BlueprintCallable)
		void SetPlayerMouse(bool Active);

	void StartGame();
	void GetMineralGrace(int mineral, int grace);
	void UpgradeHp(int addHp);
#pragma region ServerFunctions


	// MULTICAST
		// 피격 달했을 때 처리
	UFUNCTION(BlueprintCallable, Category = Health)
		void OnHitEvent(int damage);


	void PlayMontage(class UAnimMontage* AM, FName section = "");
	bool IsPlayingMontage(class UAnimMontage* AM);

	void AddItemInServer(class AItem* item);

	void UpdateAttackAndHp(bool updateAttack, float value);

	UFUNCTION(BlueprintImplementableEvent)
		void ClickBPWidget(bool isPressed);

	UFUNCTION(BlueprintImplementableEvent)
		void InitWidgetInteraction();
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
	class UAbilityUpgradeWidget* myAbilityWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UDamageWidget> damageWidgetFactory;
	TArray<class UDamageWidget*> damageWidgets;
	int damageWidgetCount = 30;
	int damageWidgetIndex = 0;

public:
	class ATPSPlayerController* myController;
	FTimerHandle TickIdTimerHandle;

// 
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

	UPROPERTY(EditAnywhere)
		class UItemFactoryComp* itemFactory;

	UPROPERTY(EditAnywhere, blueprintReadOnly)
		class UPlayerAbilityComp* abilityComp;

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
	
	UPROPERTY(BlueprintReadWrite)
		int Grace = 10000;

	UPROPERTY(BlueprintReadWrite)
		int Mineral = 10000;

	UPROPERTY(BlueprintReadOnly)
		bool isBought = false;

	UPROPERTY(Replicated)
	class ABuildableItem* buildableItem;
	TArray<class ABuildableItem*> ItemArr;


// 함수========================================================================
	UFUNCTION(BlueprintCallable)
		void BuyItem(int32 itemId, int ItemGrace, int ItemMineral, int32 ItemCount);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
		void OnGameOver();
		void OnGameOver_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = Initialization)
		void OnInitialization();
	FTimerHandle addItemTimer;
	class UInventory* GetInventory();
	void AddHP(int value);
};
