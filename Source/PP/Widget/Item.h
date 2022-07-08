// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UItem : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_IconImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_CraftingAmount;

	FString m_ItemName;
	int32	m_BaseCraftingAmount;

	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetName(FString _Name) { m_ItemName = _Name; }
	void SetImage(UTexture2D* _Texture) { m_IconImage->SetBrushFromTexture(_Texture); }
	void SetAmount(int32 _Amount) { m_CraftingAmount->SetText(FText::FromString(FString::FromInt(_Amount))); }
	void SetBaseCraftingAmount(int32 _Amount) { m_BaseCraftingAmount = _Amount; }

	FString GetItemName() { return m_ItemName; }
	int32 GetBaseCraftingAmount() { return m_BaseCraftingAmount; }
};
