// Fill out your copyright notice in the Description page of Project Settings.


#include "UseItemTile.h"
#include "InventoryItemDataTile.h"

void UUseItemTile::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	m_Frame = Cast<UImage>(GetWidgetFromName(TEXT("Frame")));
	m_Amount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Amount")));
	m_DurabilityBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DurabilityBar")));

	m_BaseTexture = LoadObject<UTexture2D>(nullptr,
		TEXT("Texture2D'/Game/CharacterCustomizer/Character_Customizer/UMG/Icons/Icon_Transparent.Icon_Transparent'"));

	m_FrameOnTexture = LoadObject<UTexture2D>(nullptr,
		TEXT("Texture2D'/Game/EasySurvivalRPG/Textures/UI/T_UI_Button_Frame_003.T_UI_Button_Frame_003'"));
	m_FrameOffTexture = LoadObject<UTexture2D>(nullptr,
		TEXT("Texture2D'/Game/EasySurvivalRPG/Textures/UI/T_UI_Button_Frame_002.T_UI_Button_Frame_002'"));
}


void UUseItemTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUseItemTile::SetData(UObject* Data)
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
	if (IconTexture)
		m_IconImage->SetBrushFromTexture(IconTexture);
	else
		m_IconImage->SetBrushFromTexture(m_BaseTexture);
	
	if(InvenData->GetUse())
		m_Frame->SetBrushFromTexture(m_FrameOnTexture);
	else
		m_Frame->SetBrushFromTexture(m_FrameOffTexture);
		
}

