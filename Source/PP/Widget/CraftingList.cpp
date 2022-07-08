// Fill out your copyright notice in the Description page of Project Settings.



#include "CraftingList.h"
#include "InventoryItemDataTile.h"
#include "../PPGameInstance.h"
#include "../PPGameModeBase.h"
void UCraftingList::NativeConstruct()
{
	Super::NativeConstruct();
	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	m_BackgroundIcon = Cast<UImage>(GetWidgetFromName(TEXT("BackgroundIcon")));
	m_Name = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NameText")));
	m_MaterialsText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Materials")));
}

void UCraftingList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCraftingList::SetData(UObject* Data)
{
	UInventoryItemDataTile* InvenData = Cast<UInventoryItemDataTile>(Data);
	UTexture2D* IconTexture = InvenData->GetIconTexture();
	m_IconImage->SetBrushFromTexture(IconTexture);
	m_Name->SetText(FText::FromString(InvenData->GetNameText()));
	m_Index = InvenData->GetIndex();

	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(InvenData->GetNameText());
	FString Name;
	for (int32 i = 0; i < Info->MakeMaterials.Num(); i++)
	{
		Name += Info->MakeMaterials[i].ItemName + TEXT(" ") + FString::FromInt(Info->MakeMaterials[i].aMount) + TEXT("  ");
	}
	m_MaterialsText->SetText(FText::FromString(Name));
	m_BackgroundIcon->SetColorAndOpacity(InvenData->GetColor());
}
