// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/TileView.h"
#include "Components/CanvasPanel.h"
#include "InventoryItemDataTile.h"

#include "UseInventoryTile.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UUseInventoryTile : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView*    m_UseInventorySlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<bool> m_UseSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APlayerCharacter* m_Player;

	bool	m_MouseHovered;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION()
		void ItemClick(UObject* Data);
public:
	void AddItem(const FUIItemTableInfo* ItemInfo, int32 ItemAMount = 1);
	bool IsFull();
	void ItemCharacterpass(int32 _Num, bool _Pass);
	void ItemUse(int32 _Num);
	void UseOn(int32 _Num, bool _Use);

	void SetPlayer(class APlayerCharacter* _Player);
};
