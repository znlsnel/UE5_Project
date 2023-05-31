// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerUI : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	virtual void InitializeComponent()override;
	virtual void BeginPlay()override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent)override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;
	void GetMouseInput();
	void ToggleMouse();
	void ToggleMouse(bool ActiveMouse);

	template <bool b>
	void ATVWeaponSelectUI() { ATVWeaponSelectUI(b); }
	void ATVWeaponSelectUI(bool addToView);

	UFUNCTION(Client, Reliable)
	void ATVWidgets();
	void ATVWidgets_Implementation();

	void InitializeWidgets();
	UFUNCTION(Client, Reliable)
		void InitializeWidgets_Client();
		void InitializeWidgets_Client_Implementation();

	UFUNCTION(Server, Reliable)
		void InitializeWidgets_Server();
		void InitializeWidgets_Server_Implementation();

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UScreenUI> ScreenUIFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UScreenUI* screenUI;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCrosshair> crosshairFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UCrosshair* crosshair;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UWeaponUI> weaponUIFactory;
	UPROPERTY(blueprintReadWrite, category = UI)
		class UWeaponUI* weaponSelectUI;

	bool IsMouseActive = false;


	// NoneOnOff : 0 -> Toggle, 1 -> On, 2 -> Off
	void ToggleInventory();
	void ToggleInventory(bool On);
};
