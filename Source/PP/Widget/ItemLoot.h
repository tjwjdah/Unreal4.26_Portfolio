// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "ItemLoot.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UItemLoot : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UListView* m_LootList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<bool> m_UseSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APlayerCharacter* m_Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AItemBox* m_AItemBox;
	bool	m_MouseHovered;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION()
		void ItemClick(UObject* Data);

public:
	void AddItem(const FUIItemTableInfo* ItemInfo, int32 ItemAMount = 1);

	void DeleteItem();
	/*
	bool IsFull();
	void ItemCharacterpass(int32 _Num, bool _Pass);
	void ItemUse(int32 _Num);
	void UseOn(int32 _Num, bool _Use);
	*/
	UFUNCTION()
	void CloseButtonClick();

	void SetItemBox(class AItemBox* _ItemBox);

	void ItemRollBack();
};
