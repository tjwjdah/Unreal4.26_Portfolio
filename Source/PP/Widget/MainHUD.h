// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "HotKey.h"
#include "Crosshair.h"
#include "CharacterState.h"
#include "Components/Border.h"
#include "InventoryTile.h"
#include "UseInventoryTile.h"
#include "ItemLoot.h"
#include "Crafting.h"
#include "DurabilityBar.h"
#include "SelectedBlueprintInfo.h"
#include "UseGarmentInventoryTile.h"
#include "MainHUD.generated.h"
/**
 * 
 */
UCLASS()
class PP_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCrosshair* m_Crosshair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UInventoryTile* m_Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUseInventoryTile* m_UseInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUseGarmentInventoryTile* m_UseGarmentInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBorder* m_UIOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UItemLoot* m_Loot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UHotKey* m_HotKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCharacterState* m_CharacterState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDurabilityBar* m_DurabilityBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCrafting* m_Crafting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USelectedBlueprintInfo* m_SelectedBlueprintInfo;
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UCrosshair* GetCrossHair() { return m_Crosshair; }
	UInventoryTile* GetInventory() { return m_Inventory; }
	UUseInventoryTile* GetUseInventory() { return m_UseInventory; }
	UUseGarmentInventoryTile* GetGarmentInventory() { return m_UseGarmentInventory; }
	UItemLoot* GetLoot() { return m_Loot; }
	UHotKey* GetHotKey() { return m_HotKey; }
	UCharacterState* GetCharacterState() { return m_CharacterState; }
	UDurabilityBar* GetDurabilityBar() { return m_DurabilityBar; }
	UCrafting* GetCrafting() { return m_Crafting; }
	USelectedBlueprintInfo* GetSelectedBlueprintInfo() { return m_SelectedBlueprintInfo; }
	void UI_OnOff(bool _On);
};
