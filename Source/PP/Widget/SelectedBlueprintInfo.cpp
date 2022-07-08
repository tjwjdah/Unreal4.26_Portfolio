// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedBlueprintInfo.h"
#include "InventoryTile.h"
#include "Item.h"
#include "../PPGameInstance.h"
#include "../PPGameModeBase.h"
#include "Engine/BlueprintGeneratedClass.h"
void USelectedBlueprintInfo::NativeConstruct()
{
	Super::NativeConstruct();

	m_FrameImage = Cast<UImage>(GetWidgetFromName(TEXT("Frame")));

	m_SelectedBlueprintDescription= Cast<UTextBlock>(GetWidgetFromName(TEXT("SelectedBlueprintDescription")));

	m_RequiredItemsBox= Cast<UWrapBox>(GetWidgetFromName(TEXT("RequiredItemsBox")));
	m_ResultItemsBox =Cast<UWrapBox>(GetWidgetFromName(TEXT("ResultItemsBox")));
	m_SelectedBlueprintName = Cast<UTextBlock>(GetWidgetFromName(TEXT("SelectedBlueprintName")));
	m_CraftingAmount = Cast<UTextBlock>(GetWidgetFromName(TEXT("CraftingAmount")));
	m_BtnDecreaseAmount = Cast<UUIButton>(GetWidgetFromName(TEXT("BtnDecreaseAmount")));

	m_BtnIncreaseAmount = Cast<UUIButton>(GetWidgetFromName(TEXT("BtnIncreaseAmount")));

	m_BtnCraft = Cast<UUIButton>(GetWidgetFromName(TEXT("Craft")));

	m_BtnDecreaseAmount->GetButton()->OnClicked.AddDynamic(this, &USelectedBlueprintInfo::DecreaseAmount);
	m_BtnIncreaseAmount->GetButton()->OnClicked.AddDynamic(this, &USelectedBlueprintInfo::IncreaseAmount);
	m_BtnCraft->GetButton()->OnClicked.AddDynamic(this, &USelectedBlueprintInfo::Craft);
	m_SelectOn = false;

	m_CraftCheck = false;
}


void USelectedBlueprintInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USelectedBlueprintInfo::SelectedBlueprint(const FUIItemTableInfo* ItemInfo)
{
	m_SelectOn = true;
	m_MakeAmount = 1;
	m_RequiredItemsBox->ClearChildren();
	m_ResultItemsBox->ClearChildren();
	m_FrameImage->SetBrushFromTexture(ItemInfo->IconTexture);
	
	
	
	/*UBlueprintGeneratedClass* item3 = LoadObject<UBlueprintGeneratedClass>(nullptr,
		TEXT("WidgetBlueprint'/Game/UI/UI_Item.UI_Item_C'"));
	
	UClass* Item2 = Cast<UClass>(item3);
	
	UItem* Item = Cast<UItem>(Item2->GetDefaultObject());
		/*
	item = LoadObject<UItem>(nullptr,
		TEXT("WidgetBlueprint'/Game/UI/UI_Item.UI_Item_C'"));
	item->GetClass();
	*/
	m_SelectedBlueprintName->SetText(FText::FromString(ItemInfo->Name));
	
	m_SelectedBlueprintDescription->SetText(FText::FromString(ItemInfo->Desc));
	
	FStringClassReference MyWidgetClassRef(TEXT("WidgetBlueprint'/Game/UI/UI_Item.UI_Item_C'"));
	UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UItem>();
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	for (int32 i = 0; i < ItemInfo->MakeMaterials.Num(); i++)
	{
		UItem* Item = CreateWidget<UItem>(GetWorld(), MyWidgetClass);
		Item->AddToViewport();
		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(ItemInfo->MakeMaterials[i].ItemName);
		Item->SetName(ItemInfo->MakeMaterials[i].ItemName);
		Item->SetImage(Info->IconTexture);
		Item->SetAmount(ItemInfo->MakeMaterials[i].aMount);
		Item->SetBaseCraftingAmount(ItemInfo->MakeMaterials[i].aMount);
		m_RequiredItemsBox->AddChild(Item);
	}

	UItem* Item = CreateWidget<UItem>(GetWorld(), MyWidgetClass);
	Item->AddToViewport();
	Item->SetName(ItemInfo->Name);
	Item->SetImage(ItemInfo->IconTexture);
	Item->SetAmount(m_MakeAmount);
	m_ResultItemsBox->AddChild(Item);

	m_CraftingAmount->SetText(FText::FromString(FString::FromInt(m_MakeAmount)));
	//Item->SetImage()
}

void USelectedBlueprintInfo::DecreaseAmount()
{
	if (!m_SelectOn)
		return;
	if (m_MakeAmount > 1)
	{
		/*
		m_RequiredItemsBox->ClearChildren();
		m_ResultItemsBox->ClearChildren();
		*/
		m_MakeAmount--;
		for (int32 i = 0; i < m_RequiredItemsBox->GetChildrenCount(); i++)
		{
			UItem* Item =Cast<UItem>(m_RequiredItemsBox->GetChildAt(i));
			Item->SetAmount(Item->GetBaseCraftingAmount() * m_MakeAmount);
		}
		UItem* Item2 = Cast<UItem>(m_ResultItemsBox->GetChildAt(0));
		Item2->SetAmount(m_MakeAmount);
	
		m_CraftingAmount->SetText(FText::FromString(FString::FromInt(m_MakeAmount)));
	}
}

void USelectedBlueprintInfo::IncreaseAmount()
{
	if (!m_SelectOn)
		return;
	m_MakeAmount++;
	for (int32 i = 0; i < m_RequiredItemsBox->GetChildrenCount(); i++)
	{
		UItem* Item = Cast<UItem>(m_RequiredItemsBox->GetChildAt(i));
		Item->SetAmount(Item->GetBaseCraftingAmount() * m_MakeAmount);
	}
	UItem* Item2 = Cast<UItem>(m_ResultItemsBox->GetChildAt(0));
	Item2->SetAmount(m_MakeAmount);
	m_CraftingAmount->SetText(FText::FromString(FString::FromInt(m_MakeAmount)));
}

void USelectedBlueprintInfo::Craft()
{
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode->GetMainHUD()->GetInventory()->IsFull())
		return;

	UTileView* Inven = GameMode->GetMainHUD()->GetInventory()->GetInventorySlot();
	
	m_CraftCheck = true;

	for (int32 i = 0; i < m_RequiredItemsBox->GetChildrenCount(); i++)
	{
		UItem* CraftItem = Cast<UItem>(m_RequiredItemsBox->GetChildAt(i));
		
		for (int32 l = 0; l < 20; l++)
		{
			UInventoryItemDataTile* InvenItem = Cast<UInventoryItemDataTile>(Inven->GetItemAt(l));
			if (InvenItem->GetNameText() == CraftItem->GetItemName())
			{
				if (InvenItem->GetAMount() < CraftItem->GetBaseCraftingAmount() * m_MakeAmount)
					m_CraftCheck = false;
			}
			//m_RequiredItemsBox
		}	
	}
	if (!m_CraftCheck)
		return;

	for (int32 i = 0; i < m_RequiredItemsBox->GetChildrenCount(); i++)
	{
		UItem* CraftItem = Cast<UItem>(m_RequiredItemsBox->GetChildAt(i));

		for (int32 l = 0; l < 20; l++)
		{
			UInventoryItemDataTile* InvenItem = Cast<UInventoryItemDataTile>(Inven->GetItemAt(l));
			if (InvenItem->GetNameText() == CraftItem->GetItemName() )
			{
				if (InvenItem->GetAMount() == CraftItem->GetBaseCraftingAmount() * m_MakeAmount)
				{
					InvenItem->SetNameText(TEXT(""));
					InvenItem->SetIconTexture(nullptr);
					
					InvenItem->SetUsePossible(false);
					InvenItem->SetMaxAMount(0);
					InvenItem->SetAMount(0);
					InvenItem->SetItemIndex(0);
					GameMode->GetMainHUD()->GetInventory()->SetUseItemSlot(true, l);
				}
				else
				{
					InvenItem->SetAMount(InvenItem->GetAMount() - (CraftItem->GetBaseCraftingAmount() * m_MakeAmount));
				}
				
				break;
			}
		}
		
	}
	Inven->RegenerateAllEntries();
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());

	UItem* Item2 = Cast<UItem>(m_ResultItemsBox->GetChildAt(0));
	const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Item2->GetItemName());

	GameMode->GetMainHUD()->GetInventory()->AddItem(Info, m_MakeAmount);

}
