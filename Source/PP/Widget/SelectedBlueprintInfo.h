// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "UIButton.h"
#include "InventoryItemDataTile.h"
#include "SelectedBlueprintInfo.generated.h"


/**
 * 
 */
UCLASS()
class PP_API USelectedBlueprintInfo : public UUserWidget
{

	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_FrameImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_SelectedBlueprintName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_SelectedBlueprintDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWrapBox* m_RequiredItemsBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWrapBox* m_ResultItemsBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_CraftingAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnDecreaseAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnIncreaseAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnCraft;
	int32		m_MakeAmount;
	bool		m_SelectOn;

	bool		m_CraftCheck;
    //class	UItem* item;
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SelectedBlueprint(const FUIItemTableInfo* ItemInfo);

	UFUNCTION()
	void DecreaseAmount();
	UFUNCTION()
	void IncreaseAmount();
	UFUNCTION()
	void Craft();

};
