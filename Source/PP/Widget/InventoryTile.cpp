// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTile.h"
#include "../PPGameInstance.h"
#include "Components/CanvasPanelSlot.h"
#include "../PPGameModeBase.h"

#include "MainHUD.h"
void UInventoryTile::NativeConstruct()
{
	Super::NativeConstruct();

	
	
	m_InventorySlot = Cast<UTileView>(GetWidgetFromName(TEXT("InventorySlot")));
	m_ItemToolTip = Cast<UItemToolTip>(GetWidgetFromName(TEXT("UI_ItemToolTip")));
	m_InventorySlot->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	for (int32 i = 0; i < 20; ++i)
	{
		UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,
			UInventoryItemDataTile::StaticClass());

		Data->SetNameText(TEXT(""));
		Data->SetIconTexture(nullptr);
		Data->SetIndex(i);
		Data->SetUsePossible(false);
		Data->SetMaxAMount(0);
		Data->SetAMount(0);
		Data->SetItemIndex(0);
		m_InventorySlot->AddItem(Data);
		m_UseSlot.Add(true);

	}
	
	m_InventorySlot->OnItemClicked().AddUObject(this, &UInventoryTile::ItemClick);
	m_InventorySlot->OnItemIsHoveredChanged().AddUObject(this, &UInventoryTile::ItemHOvered);
	
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	FString d = TEXT("Wood");
	const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 20);
	}
	d = TEXT("Metal");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 20);
	}
	d = TEXT("Stone");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 20);
	}
	
	d = TEXT("PlateArmor");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info,1);
	}
	
	d = TEXT("Bow");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 1);
	}
	d = TEXT("Sword");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 1);
	}
	/*
	d = TEXT("WoodDoorFrame");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 30);
	}
	d = TEXT("WoodWIndowFrame");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 30);
	}
	d = TEXT("WoodRamp");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 30);
	}
	d = TEXT("WoodStairs");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 30);
	}
	
	d = TEXT("WoodFence");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 30);
	}
	d = TEXT("WoodDoor");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 20);
	}
	d = TEXT("WoodWindow");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 20);
	}
	
	d = TEXT("leather");
	Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		AddItem(Info, 50);
	}
	*/
	m_MouseHovered = false;

	m_MouseIndex = 0;
}


void UInventoryTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	/*
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	FString d = TEXT("Bow");
	const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(d);
	if (Info)
	{
		PrintViewport(1.f, FColor::Red, d);
		//AddItem(Info, 3);
	}
	*/


	
}

FReply UInventoryTile::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	if (m_MouseHovered)
	{
		//FVector2D	S

		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemToolTip->Slot);

		if (PanelSlot)
		{
			FVector2D	WidgetPos = InMouseEvent.GetScreenSpacePosition();

			//USlateBlueprintLibrary::ScreenToWidgetLocal(this, InGeometry,
			//	InMouseEvent.GetScreenSpacePosition(), WidgetPos);

			// 마우스의 Screen좌표를 위젯의 Local 좌표로 변경한다.
			WidgetPos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry,
				WidgetPos);

			//WidgetPos.X += 1.f;
			
			if (m_MouseIndex >= 5)
				WidgetPos.Y -= 450.f;
			else
				WidgetPos.Y -= 20.f;

			PanelSlot->SetPosition(WidgetPos);
			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("X : %.5f Y : %.5f"), m_MouseX, m_MouseY));
		}
	}

	return FReply::Handled();
}

void UInventoryTile::ItemClick(UObject* Data)
{
	UInventoryItemDataTile* DataItem = Cast<UInventoryItemDataTile>(Data);
	
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	
	
	if (DataItem)
	{

		if (DataItem->GetUsePossible())
		{
			
			UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
			const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(DataItem->GetNameText());
			if (Info->ItemType == EItemType::Equipment)
			{
				m_UseGarmentInventory = GameMode->GetMainHUD()->GetGarmentInventory();

				m_UseSlot[DataItem->GetIndex()] = true;

				m_UseGarmentInventory->AddItem(Info, DataItem->GetAMount());

				UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(DataItem->GetIndex()));
				Item->SetNameText(TEXT(""));
				Item->SetIconTexture(nullptr);
				Item->SetUsePossible(false);
				Item->SetMaxAMount(0);
				Item->SetAMount(0);
				Item->SetItemIndex(0);
				m_InventorySlot->RegenerateAllEntries();
			}
			else
			{
				m_UseInventory = GameMode->GetMainHUD()->GetUseInventory();
				if (!m_UseInventory->IsFull())
				{

					
					UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(DataItem->GetIndex()));
					m_UseSlot[DataItem->GetIndex()] = true;

					m_UseInventory->AddItem(Info, DataItem->GetAMount());


					Item->SetNameText(TEXT(""));
					Item->SetIconTexture(nullptr);
					Item->SetUsePossible(false);
					Item->SetMaxAMount(0);
					Item->SetAMount(0);
					Item->SetItemIndex(0);
					m_InventorySlot->RegenerateAllEntries();
				}
			}
		}

	}
}

void UInventoryTile::ItemScroll(UObject* Data, UUserWidget* widget)
{
}

void UInventoryTile::ItemHOvered(UObject* Data, bool Hovered)
{
	/*
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Data);
	if(m_UseSlot[Item->GetIndex()])
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("1111")));
	else
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("0000")));
		*/
	
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Data);
	if (m_UseSlot[Item->GetIndex()])
	{
		m_ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
		m_MouseHovered = false;
		return;
	}
		
	m_MouseHovered = Hovered;
	
	if (Hovered)
	{
		
		m_ItemToolTip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		

		if (Item)
		{
			UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());

			const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Item->GetNameText());

			m_ItemToolTip->SetItem(Info, Item->GetAMount());
			m_MouseIndex = Item->GetIndex();
		}
	}

	else
		m_ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);

	
}

void UInventoryTile::AddItem(const FUIItemTableInfo* ItemInfo, int32 ItemAMount )
{
	
	int32 AMount = ItemAMount;
	for (int32 i = 0; i < 20; i++)
	{
		if (m_UseSlot[i])
			continue;
		UInventoryItemDataTile* Item=Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(i));
		
		if (ItemInfo->ItemIndex == Item->GetItemIndex())
		{
		
			if (!Item->IsFull())
			{
				if (Item->GetMaxAMount() < Item->GetAMount() + ItemAMount)
				{
					Item->SetAMount(Item->GetMaxAMount());
					AMount = Item->GetAMount() + ItemAMount - Item->GetMaxAMount();
				}
				else
				{
					Item->SetAMount(Item->GetAMount() + ItemAMount);
					m_InventorySlot->RegenerateAllEntries();
					return;
				}
			}

		}
	}
	int32 ItemCount = -1;
	for (int32 i = 0; i < 20; i++)
	{
		if (m_UseSlot[i])
		{
			ItemCount = i;
			break;
		}
	}

	while (AMount > ItemInfo->MaxAMount)
	{
		
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(ItemCount));
		/*
		m_InventorySlot->SetSelectedIndex(m_ItemCount);
		UInventoryItemDataTile* Item = NewObject<UInventoryItemDataTile>(this,
			UInventoryItemDataTile::StaticClass());
			*/

		Item->SetNameText(ItemInfo->Name);
		Item->SetIconTexture(ItemInfo->IconTexture);
		//Item->SetIndex(ItemCount);
		Item->SetUsePossible(ItemInfo->UsePossible);
		Item->SetMaxAMount(ItemInfo->MaxAMount);
		Item->SetAMount(ItemInfo->MaxAMount);
		Item->SetItemIndex(ItemInfo->ItemIndex);
		//m_InventorySlot->SetSelectedItem(Item);
		AMount -= ItemInfo->MaxAMount;
		m_UseSlot[ItemCount] = false;
		for (int32 i = 0; i < 20; i++)
		{
			if (m_UseSlot[i])
			{
				ItemCount = i;
				break;
			}
		}
	}
	

	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(ItemCount));
	/*
	m_InventorySlot->SetSelectedIndex(m_ItemCount);
	UInventoryItemDataTile* Item = NewObject<UInventoryItemDataTile>(this,
		UInventoryItemDataTile::StaticClass());
		*/
	Item->SetNameText(ItemInfo->Name);
	Item->SetIconTexture(ItemInfo->IconTexture);
	//Item->SetIndex(ItemCount);
	Item->SetUsePossible(ItemInfo->UsePossible);
	Item->SetMaxAMount(ItemInfo->MaxAMount);
	Item->SetAMount(AMount);
	Item->SetItemIndex(ItemInfo->ItemIndex);
	//m_InventorySlot->SetSelectedItem(Item);
	m_UseSlot[ItemCount] = false;
	m_InventorySlot->RegenerateAllEntries();
}
/*
void UInventoryTile::AddItem(UInventoryItemDataTile* Data)
{
	//PrintViewport(1.f, FColor::Red, TEXT("1"));
	for (int32 i = 0; i < 20; i++)
	{
		if (m_UseSlot[i])
			continue;
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(i));

		if (Data->GetItemIndex() == Item->GetItemIndex())
		{

			if (!Item->IsFull())
			{
				if (Item->GetMaxAMount() < Item->GetAMount() + Data->GetAMount())
				{
					Item->SetAMount(Item->GetMaxAMount());
					Data->SetAMount(Item->GetAMount() + Data->GetAMount() - Item->GetMaxAMount());
				}
				else
				{
					Item->SetAMount(Item->GetAMount() + Data->GetAMount());
					m_InventorySlot->RegenerateAllEntries();
					return;
				}
			}

		}
	}
	int32 ItemCount = -1;
	for (int32 i = 0; i < 20; i++)
	{
		if (m_UseSlot[i])
		{
			ItemCount = i;
			break;
		}
	}
	
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(ItemCount));
	/*
	m_InventorySlot->SetSelectedIndex(m_ItemCount);
	UInventoryItemDataTile* Item = NewObject<UInventoryItemDataTile>(this,
		UInventoryItemDataTile::StaticClass());
		*/
/*
	Item->SetNameText(Data->GetName());
	Item->SetIconTexture(Data->GetIconTexture());
	Item->SetIndex(ItemCount);
	Item->SetUsePossible(Data->GetUsePossible());
	Item->SetMaxAMount(Data->GetMaxAMount());
	Item->SetAMount(Data->GetAMount());
	Item->SetItemIndex(Data->GetItemIndex());
	//m_InventorySlot->SetSelectedItem(Item);
	m_UseSlot[ItemCount] = false;
	m_InventorySlot->RegenerateAllEntries();
}
*/
bool UInventoryTile::IsFull()
{
	for (int32 i = 0; i < 20; i++)
	{
		if (m_UseSlot[i])
			return false;
	}
	return true;
}

int32 UInventoryTile::FindItem(const FUIItemTableInfo* ItemInfo)
{
	for (int32 i = 0; i < 20; i++)
	{
		if (m_UseSlot[i])
			continue;
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_InventorySlot->GetItemAt(i));

		if (ItemInfo->ItemIndex == Item->GetItemIndex())
			return i;
	}

	return -1;
}

