// Fill out your copyright notice in the Description page of Project Settings.


#include "UseInventoryTile.h"
#include "InventoryTile.h"
#include "../PPGameInstance.h"
#include "Components/CanvasPanelSlot.h"
#include "../PPGameModeBase.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Equipment/UseItem.h"
#include "../Equipment/Weapon.h"
#include "../Equipment/Bow.h"
#include "../Equipment/UseBuildable.h"
#include "../Equipment/Pick.h"
#include "../Equipment/Axe.h"
#include "../Equipment/Sword.h"
#include "../Equipment/EatItem.h"
#include "../Equipment/Garment.h"
void UUseInventoryTile::NativeConstruct()
{
	Super::NativeConstruct();


	m_UseInventorySlot = Cast<UTileView>(GetWidgetFromName(TEXT("UseItem")));
	m_UseInventorySlot->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	for (int32 i = 0; i < 6; ++i)
	{
		UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,
			UInventoryItemDataTile::StaticClass());

		m_UseInventorySlot->AddItem(Data);
		m_UseSlot.Add(true);
	
	}
	m_UseInventorySlot->OnItemClicked().AddUObject(this, &UUseInventoryTile::ItemClick);
	//m_Player= Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UUseInventoryTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	
}

FReply UUseInventoryTile::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UUseInventoryTile::AddItem(const FUIItemTableInfo* ItemInfo, int32 ItemAMount )
{
	for (int32 i = 0; i < 6; ++i)
	{
		if (m_UseSlot[i])
		{
			UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(i));
			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), i));


			Item->SetNameText(ItemInfo->Name);
			Item->SetIconTexture(ItemInfo->IconTexture);
			Item->SetIndex(i);
			Item->SetUsePossible(ItemInfo->UsePossible);
			Item->SetMaxAMount(ItemInfo->MaxAMount);
			Item->SetAMount(ItemAMount);
			Item->SetItemIndex(ItemInfo->ItemIndex);
			//m_InventorySlot->SetSelectedItem(Item);
			m_UseInventorySlot->RegenerateAllEntries();
			m_UseSlot[i] = false;


			ItemCharacterpass(i,false);
			return;
		}
	}
}

bool UUseInventoryTile::IsFull()
{
	for (int32 i = 0; i < 6; i++)
	{
		if (m_UseSlot[i])
			return false;
	}
	return true;
}

void UUseInventoryTile::ItemCharacterpass(int32 _Num, bool _Pass)
{
	if (_Pass)
		m_Player->SetInvenItem(nullptr, _Num);
	else
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(_Num));
		UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Item->GetNameText());
		
		switch (Info->ItemType)
		{
		case EItemType::BuildItem:
		{
			AUseBuildable* MakeItem = NewObject<AUseBuildable>(m_Player,
				AUseBuildable::StaticClass());
			for (int32 i = 0; i < Info->OptionArray.Num(); i++) {
				if (Info->OptionArray[i].OptionType == EItemOption::Durability)
				{
					MakeItem->SetMaxDurability(Info->OptionArray[i].Option);
					MakeItem->SetDurability(Info->OptionArray[i].Option);
					switch (Info->OptionArray[i].Option)
					{
					case 1000: MakeItem->SetMaterialType(EnvironmentsType::Wood);
						break;
					case 2000: MakeItem->SetMaterialType(EnvironmentsType::Stone);
						break;
					case 3000: MakeItem->SetMaterialType(EnvironmentsType::Metal);
						break;
					}
				}
			}
			
			
			MakeItem->SetMesh(Info->DestructibleMesh, Info->Mesh);
			MakeItem->SetBuildType(Info->EBuildType);
			m_Player->SetInvenItem(MakeItem, _Num);
		}
		break;
		case EItemType::Weapon:
		{
			switch (Info->WeaponType)
			{
			case EWeaponType::Sword:
			{
				ASword* MakeItem = NewObject<ASword>(m_Player,
					ASword::StaticClass());
				MakeItem->SetMesh(Info->SkeletalMesh);
				for (int32 i = 0; i < Info->OptionArray.Num(); i++) {
					if (Info->OptionArray[i].OptionType == EItemOption::Attack)
					{
						MakeItem->SetAttack(Info->OptionArray[i].Option);
						//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), Info->OptionArray[i].Option));
						
					}
					else if (Info->OptionArray[i].OptionType == EItemOption::BuildAttack)
					{
						MakeItem->SetBuildAttack(Info->OptionArray[i].Option);
					}
				}

				m_Player->SetInvenItem(MakeItem, _Num);
			}
				break;
			case EWeaponType::Bow:
			{
				
				ABow* MakeItem = NewObject<ABow>(m_Player,
					ABow::StaticClass());
				MakeItem->SetMesh(Info->SkeletalMesh);
				for (int32 i = 0; i<Info->OptionArray.Num(); i++) {
					if (Info->OptionArray[i].OptionType == EItemOption::Attack)
						MakeItem->SetAttack(Info->OptionArray[i].Option);
					else if (Info->OptionArray[i].OptionType == EItemOption::BuildAttack)
					{
						MakeItem->SetBuildAttack(Info->OptionArray[i].Option);
					}
				}
				
				m_Player->SetInvenItem(MakeItem, _Num);
			}
				break;
			case EWeaponType::Pick:
			{
				APick* MakeItem = NewObject<APick>(m_Player,
					APick::StaticClass());
				MakeItem->SetMesh(Info->SkeletalMesh);
				for (int32 i = 0; i < Info->OptionArray.Num(); i++) {
					if (Info->OptionArray[i].OptionType == EItemOption::Attack)
						MakeItem->SetAttack(Info->OptionArray[i].Option);
					else if (Info->OptionArray[i].OptionType == EItemOption::BuildAttack)
					{
						MakeItem->SetBuildAttack(Info->OptionArray[i].Option);
					}
				}

				m_Player->SetInvenItem(MakeItem, _Num);
			}
				break;
			case EWeaponType::Axe:
			{
				AAxe* MakeItem = NewObject<AAxe>(m_Player,
					AAxe::StaticClass());
				MakeItem->SetMesh(Info->SkeletalMesh);
				for (int32 i = 0; i < Info->OptionArray.Num(); i++) {
					if (Info->OptionArray[i].OptionType == EItemOption::Attack)
						MakeItem->SetAttack(Info->OptionArray[i].Option);
					else if (Info->OptionArray[i].OptionType == EItemOption::BuildAttack)
					{
						MakeItem->SetBuildAttack(Info->OptionArray[i].Option);
					}
				}

				m_Player->SetInvenItem(MakeItem, _Num);
			}
				break;
			case EWeaponType::None:
				break;
			default:
				break;
			}
		}
			break;
		case EItemType::Equipment:
		{
			AGarment* MakeItem = NewObject<AGarment>(m_Player,
				AGarment::StaticClass());
			for (int32 i = 0; i < Info->OptionArray.Num(); i++) {
				if (Info->OptionArray[i].OptionType == EItemOption::Armor)
					MakeItem->SetArmor(Info->OptionArray[i].Option);
			}

			m_Player->SetInvenItem(MakeItem, _Num);
		}
			break;
		case EItemType::Eat:
		{
			AEatItem* MakeItem = NewObject<AEatItem>(m_Player,
				AEatItem::StaticClass());
			for (int32 i = 0; i < Info->OptionArray.Num(); i++) {
				MakeItem->SetOptions(Info->OptionArray[i]);
			}

			m_Player->SetInvenItem(MakeItem, _Num);

		}
			break;
		}


	}
}



void UUseInventoryTile::ItemClick(UObject* Data)
{
	UInventoryItemDataTile* DataItem = Cast<UInventoryItemDataTile>(Data);
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	UInventoryTile* Inven =GameMode->GetMainHUD()->GetInventory();
	if (DataItem)
	{
		if (DataItem->GetUsePossible())
		{
			if (!Inven->IsFull())
			{

				//UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(DataItem->GetIndex()));

				m_UseSlot[DataItem->GetIndex()] = true;
				UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
				const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(DataItem->GetNameText());
				Inven->AddItem(Info, DataItem->GetAMount());
				ItemCharacterpass(DataItem->GetIndex() , true);

				DataItem->SetNameText(TEXT(""));
				DataItem->SetIconTexture(nullptr);
			
				DataItem->SetUsePossible(false);
				DataItem->SetMaxAMount(0);
				DataItem->SetAMount(0);
				DataItem->SetItemIndex(0);
				m_UseInventorySlot->RegenerateAllEntries();
				m_UseSlot[DataItem->GetIndex()] = true;
			}
		}
	}
}


void UUseInventoryTile::ItemUse(int32 _Num)
{
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(_Num));
	
	Item->SetAMount(Item->GetAMount()-1);
	if (Item->GetAMount() == 0)
	{
		
		m_Player->SetInvenItem(nullptr, Item->GetIndex());
		Item->SetNameText(TEXT(""));
		Item->SetIconTexture(nullptr);
		
		Item->SetUsePossible(false);
		Item->SetMaxAMount(0);
		Item->SetAMount(0);
		Item->SetItemIndex(0);
		m_UseSlot[Item->GetIndex()] = true;
	}
	//m_InventorySlot->SetSelectedItem(Item);
	m_UseInventorySlot->RegenerateAllEntries();

	return;
}

void UUseInventoryTile::UseOn(int32 _Num, bool _Use)
{
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_UseInventorySlot->GetItemAt(_Num));

	Item->SetUse(_Use);
	
	//m_InventorySlot->SetSelectedItem(Item);
	m_UseInventorySlot->RegenerateAllEntries();

	return;
}
void UUseInventoryTile::SetPlayer(class APlayerCharacter* _Player) {
	m_Player = _Player;
}