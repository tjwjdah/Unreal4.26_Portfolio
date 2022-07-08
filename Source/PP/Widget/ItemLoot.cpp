// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLoot.h"
#include "InventoryItemDataTile.h"
#include "../PPGameInstance.h"
#include "../PPGameModeBase.h"
#include "../Tools/ItemBox.h"
#include "InventoryTile.h"
void UItemLoot::NativeConstruct()
{
	Super::NativeConstruct();

	m_LootList = Cast<UListView>(GetWidgetFromName(TEXT("LootList")));
	m_LootList->OnItemClicked().AddUObject(this, &UItemLoot::ItemClick);

	m_CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Close")));
	m_CloseButton->OnClicked.AddDynamic(this, &UItemLoot::CloseButtonClick);
}

void UItemLoot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}


void UItemLoot::ItemClick(UObject* Data)
{
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Data);
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	UInventoryTile* Inven = GameMode->GetMainHUD()->GetInventory();
	if (Item)
	{
		if (!Inven->IsFull())
		{
			UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
			const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Item->GetNameText());
			Inven->AddItem(Info, Item->GetAMount());
			m_LootList->RemoveItem(Data);
		}
	}
}
void UItemLoot::AddItem(const FUIItemTableInfo* ItemInfo, int32 ItemAMount) {
	
	UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,
		UInventoryItemDataTile::StaticClass());

	Data->SetNameText(ItemInfo->Name);
	Data->SetIconTexture(ItemInfo->IconTexture);
	Data->SetIndex(m_LootList->GetNumItems());
	Data->SetUsePossible(ItemInfo->UsePossible);
	Data->SetMaxAMount(ItemInfo->MaxAMount);
	Data->SetAMount(ItemAMount);
	Data->SetItemIndex(ItemInfo->ItemIndex);

	m_LootList->AddItem(Data);
}



void UItemLoot::DeleteItem() {
	while (m_LootList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_LootList->GetItemAt(0));
		m_LootList->RemoveItem(Item);
	}

}

void UItemLoot::CloseButtonClick()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Close"));
	ItemRollBack();
	DeleteItem();
	SetVisibility(ESlateVisibility::Collapsed);
	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	pController->SetInputMode(FInputModeGameOnly());
	pController->bShowMouseCursor = false;
	if (m_AItemBox != nullptr)
	{
		m_AItemBox->ItemBoxClose();
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Close")));

	}
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		GameMode->GetMainHUD()->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("Loot"))));
	}
	m_AItemBox = nullptr;
}

void UItemLoot::SetItemBox(AItemBox* _ItemBox) {
	m_AItemBox = _ItemBox;
}

void UItemLoot::ItemRollBack() {
	if (m_AItemBox)
	{
		for (int32 i = 0; i < m_LootList->GetNumItems(); i++)
		{
			UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_LootList->GetItemAt(i));
			FBoxItems ItemData;
			ItemData.aMount = Item->GetAMount();
			ItemData.ItemName = Item->GetNameText();
			m_AItemBox->AddItem(ItemData);
		}
	}
}
