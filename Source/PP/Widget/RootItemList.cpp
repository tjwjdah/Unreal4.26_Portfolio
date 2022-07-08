// Fill out your copyright notice in the Description page of Project Settings.


#include "RootItemList.h"
#include "InventoryItemDataTile.h"
void URootItemList::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));

	m_Amount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Amount")));
	m_Name = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NameText")));


}

void URootItemList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void URootItemList::SetData(UObject* Data)
{
	UInventoryItemDataTile* InvenData = Cast<UInventoryItemDataTile>(Data);
	UTexture2D* IconTexture = InvenData->GetIconTexture();
	if (InvenData->GetMaxAMount() > 1)
	{
		m_Amount->SetText(FText::FromString(FString::Printf(TEXT("%d"), InvenData->GetAMount())));
	}
	else
	{
		m_Amount->SetText(FText::FromString(FString::Printf(TEXT(""))));
	}
	m_IconImage->SetBrushFromTexture(IconTexture);
	m_Name->SetText(FText::FromString(InvenData->GetNameText()));

}
