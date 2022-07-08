// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/TileView.h"
#include "Components/CanvasPanel.h"
#include "InventoryItemTile.h"
#include "InventoryItemDataTile.h"
#include "UseInventoryTile.h"
#include "UseGarmentInventoryTile.h"
#include "ItemToolTip.h"
#include "InventoryTile.generated.h"

/**
 * 
 */


UCLASS()
class PP_API UInventoryTile : public UUserWidget
{
	GENERATED_BODY()

protected:

	UTexture2D* TestTexture;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		//UTileView* m_InventoryTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* m_InventorySlot;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//	UItemDescWidget* m_ItemDescWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UUseInventoryTile* m_UseInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UUseGarmentInventoryTile* m_UseGarmentInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<bool> m_UseSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UItemToolTip* m_ItemToolTip;
	
	//int32   m_SelectCount;
	bool	m_MouseHovered;

	int32  m_MouseIndex;
public:

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

public:
	UFUNCTION()
		void ItemClick(UObject* Data);

	UFUNCTION()
		void ItemScroll(UObject* Data, UUserWidget* widget);

	UFUNCTION()
		void ItemHOvered(UObject* Data, bool Hovered);

public:
	void AddItem(const FUIItemTableInfo* ItemInfo,int32 ItemAMount = 1);
	//void AddItem(UInventoryItemDataTile* Data);

	UTileView* GetInventorySlot() { return m_InventorySlot; }
	void SetUseItemSlot(bool _Use, int32 Num) { m_UseSlot[Num] = _Use; }
	bool IsFull();

	int32 FindItem(const FUIItemTableInfo* ItemInfo);

	void SetUseSlot(int32 _Index, bool _Use) { m_UseSlot[_Index] = _Use; }
};
