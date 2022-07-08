// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemTile.h"
#include "InventoryItemDataTile.h"

void UInventoryItemTile::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	m_Amount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Amount")));
	m_DurabilityBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DurabilityBar")));


	
	m_BaseTexture =  LoadObject<UTexture2D>(nullptr,
		TEXT("Texture2D'/Game/CharacterCustomizer/Character_Customizer/UMG/Icons/Icon_Transparent.Icon_Transparent'"));
}

void UInventoryItemTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryItemTile::SetData(UObject* Data)
{
	
	UInventoryItemDataTile* InvenData = Cast<UInventoryItemDataTile>(Data);
	UTexture2D* IconTexture = InvenData->GetIconTexture();
	if (InvenData->GetMaxAMount() > 1)
	{
		m_Amount->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), InvenData->GetAMount(), InvenData->GetMaxAMount())));
	}
	else
	{
		m_Amount->SetText(FText::FromString(FString::Printf(TEXT(""))));
	}
	if (IconTexture)
		m_IconImage->SetBrushFromTexture(IconTexture);
	else
		m_IconImage->SetBrushFromTexture(m_BaseTexture);
}

