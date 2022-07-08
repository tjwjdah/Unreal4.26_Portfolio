// Fill out your copyright notice in the Description page of Project Settings.


#include "UseGarmentInventoryTile.h"
#include "InventoryTile.h"
#include "../PPGameInstance.h"
#include "Components/CanvasPanelSlot.h"
#include "../PPGameModeBase.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Equipment/Garment.h"
void UUseGarmentInventoryTile::NativeConstruct()
{
	Super::NativeConstruct();


	m_UseInventorySlot = Cast<UTileView>(GetWidgetFromName(TEXT("UseItem")));
	m_UseInventorySlot->SetScrollbarVisibility(ESlateVisibility::Collapsed);


	UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,UInventoryItemDataTile::StaticClass());

	m_UseInventorySlot->AddItem(Data);
		
	m_UseInventorySlot->OnItemClicked().AddUObject(this, &UUseGarmentInventoryTile::ItemClick);

	m_UseGarment = true;
	//m_Player= Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UUseGarmentInventoryTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



void UUseGarmentInventoryTile::ItemClick(UObject* Data)
{
	UInventoryItemDataTile* DataItem = Cast<UInventoryItemDataTile>(Data);
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	UInventoryTile* Inven = GameMode->GetMainHUD()->GetInventory();
	if (DataItem)
	{
		if (DataItem->GetUsePossible())
		{
			if (!Inven->IsFull())
			{

				//UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(DataItem->GetIndex()));

				m_UseGarment= true;
				UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
				const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(DataItem->GetNameText());
				Inven->AddItem(Info, DataItem->GetAMount());
				ItemCharacterpass(true);
				m_Player->SetGarmentMesh(nullptr);

				DataItem->SetNameText(TEXT(""));
				DataItem->SetIconTexture(nullptr);
				
				DataItem->SetUsePossible(false);
				DataItem->SetMaxAMount(0);
				DataItem->SetAMount(0);
				DataItem->SetItemIndex(0);
				DataItem->SetUse(false);
				m_UseInventorySlot->RegenerateAllEntries();

			}
		}
	}
}

void UUseGarmentInventoryTile::AddItem(const FUIItemTableInfo* ItemInfo, int32 ItemAMount)
{
	if (m_UseGarment)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(0));

		Item->SetNameText(ItemInfo->Name);
		Item->SetIconTexture(ItemInfo->IconTexture);
		
		Item->SetUsePossible(ItemInfo->UsePossible);
		Item->SetMaxAMount(ItemInfo->MaxAMount);
		Item->SetAMount(ItemAMount);
		Item->SetItemIndex(ItemInfo->ItemIndex);
		//m_InventorySlot->SetSelectedItem(Item);
		Item->SetUse(true);
		m_UseInventorySlot->RegenerateAllEntries();
		m_UseGarment = false;
		

		ItemCharacterpass(false);
	}
	else
	{
		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
		UInventoryTile* Inven = GameMode->GetMainHUD()->GetInventory();
		UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(0));
		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Item->GetNameText());

		Inven->AddItem(Info, Item->GetAMount());

		Item->SetNameText(ItemInfo->Name);
		Item->SetIconTexture(ItemInfo->IconTexture);
	
		Item->SetUsePossible(ItemInfo->UsePossible);
		Item->SetMaxAMount(ItemInfo->MaxAMount);
		Item->SetAMount(ItemAMount);
		Item->SetItemIndex(ItemInfo->ItemIndex);
		Item->SetUse(true);
		//m_InventorySlot->SetSelectedItem(Item);
		m_UseInventorySlot->RegenerateAllEntries();
		m_UseGarment = false;


		ItemCharacterpass(false);
	}
	
}

bool UUseGarmentInventoryTile::IsFull()
{
	return !m_UseGarment;
}

void UUseGarmentInventoryTile::ItemCharacterpass(bool _Pass)
{
	if (_Pass)
	{

		m_Player->SetGarmentMesh(nullptr);
		
	}
	else
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(0));
		UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Item->GetNameText());

		m_Player->SetGarmentMesh(Info);
	}
}

void UUseGarmentInventoryTile::SetPlayer(APlayerCharacter* _Player)
{
	m_Player = _Player;
}
