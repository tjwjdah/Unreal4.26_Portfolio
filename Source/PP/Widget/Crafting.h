// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../GameInfo.h"
#include "UIButton.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Crafting.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UCrafting : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_CategoryNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_BlueprintsCountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UListView* m_CraftingList; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnAll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnEquipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnBuildItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnEat;
	

	
	int32			m_PreIndex;

	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


public:
	UFUNCTION()
		void ItemClick(UObject* Data);

public:
	void AddItem(const FUIItemTableInfo* ItemInfo, int32 _Index);
	//void AddItem(UInventoryItemDataTile* Data);
	UFUNCTION()
	void BtnAll();
	UFUNCTION()
	void BtnWeapon();
	UFUNCTION()
	void BtnEquipment();
	UFUNCTION()
	void BtnBuildItem();
	UFUNCTION()
	void BtnEat();
};
