// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

void UItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("IconImage")));
	m_CraftingAmount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AmountText")));
	
}

void UItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
