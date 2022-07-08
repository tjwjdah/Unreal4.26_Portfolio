// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting.h"
#include "InventoryItemDataTile.h"
#include "CraftingList.h"
#include "../PPGameInstance.h"
#include "../PPGameModeBase.h"
#include "SelectedBlueprintInfo.h"

void UCrafting::NativeConstruct()
{
	Super::NativeConstruct();
	m_CategoryNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CategoryNameText")));
	m_BlueprintsCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BlueprintsCountText")));
	m_CraftingList = Cast<UListView>(GetWidgetFromName(TEXT("UI_CraftingList")));
	m_BtnAll = Cast<UUIButton>(GetWidgetFromName(TEXT("UI_BtnAll")));
	m_BtnWeapon = Cast<UUIButton>(GetWidgetFromName(TEXT("UI_BtnWeapon")));
	m_BtnEquipment = Cast<UUIButton>(GetWidgetFromName(TEXT("UI_BtnEquipment")));
	m_BtnBuildItem = Cast<UUIButton>(GetWidgetFromName(TEXT("UI_BtnBuildItem")));
	m_BtnEat = Cast<UUIButton>(GetWidgetFromName(TEXT("UI_BtnEat")));

	m_CraftingList->OnItemClicked().AddUObject(this, &UCrafting::ItemClick);
	m_BtnAll->GetButton()->OnClicked.AddDynamic(this, &UCrafting::BtnAll);
	m_BtnWeapon->GetButton()->OnClicked.AddDynamic(this, &UCrafting::BtnWeapon);
	m_BtnEquipment->GetButton()->OnClicked.AddDynamic(this, &UCrafting::BtnEquipment);
	m_BtnBuildItem->GetButton()->OnClicked.AddDynamic(this, &UCrafting::BtnBuildItem);
	m_BtnEat->GetButton()->OnClicked.AddDynamic(this, &UCrafting::BtnEat);

	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const UDataTable* Table = GameInst->GetUIItemInfo();
	FString context;
	TArray<FUIItemTableInfo*> List;
	Table->GetAllRows<FUIItemTableInfo>(context, List);
	int32 Nuum = 0;
	for (int32 i = 0; i < List.Num(); i++)
	{
		if (List[i]->MakeMaterials.Num() > 0)
		{
			AddItem(List[i], Nuum);
			Nuum++;
		}
	}
	m_CategoryNameText->SetText(FText::FromString(FString::Printf(TEXT("All"))));
	m_BlueprintsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_CraftingList->GetNumItems())));
	
	m_PreIndex = 0;
	/*
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(m_PreIndex));
	Item->SetColor(FLinearColor(0.f, 0.f, 1.0f, 1.0f));
	m_CraftingList->RegenerateAllEntries();
	*/
	
}

void UCrafting::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
}

void UCrafting::ItemClick(UObject* Data)
{
	
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	USelectedBlueprintInfo* SelectedBlueprintInfo =GameMode->GetMainHUD()->GetSelectedBlueprintInfo();
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(m_PreIndex));
	Item->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.8f));
	Item = Cast<UInventoryItemDataTile>(Data);
	Item->SetColor(FLinearColor(0.f, 0.f, 1.0f, 1.0f));
	m_PreIndex = Item->GetIndex();
	m_CraftingList->RegenerateAllEntries();
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Item->GetNameText());
	SelectedBlueprintInfo->SelectedBlueprint(Info);
	
}

void UCrafting::AddItem(const FUIItemTableInfo* ItemInfo,int32 _Index)
{
	UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,
		UInventoryItemDataTile::StaticClass());

	Data->SetNameText(ItemInfo->Name);
	Data->SetIconTexture(ItemInfo->IconTexture);
	Data->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.8f));
	Data->SetIndex(_Index);
	m_CraftingList->AddItem(Data);
}

void UCrafting::BtnAll()
{
	while (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		m_CraftingList->RemoveItem(Item);
	}
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const UDataTable* Table = GameInst->GetUIItemInfo();
	FString context;
	TArray<FUIItemTableInfo*> List;
	Table->GetAllRows<FUIItemTableInfo>(context, List);
	int32 Nuum = 0;
	for (int32 i = 0; i < List.Num(); i++)
	{
		if (List[i]->MakeMaterials.Num() > 0)
		{
			AddItem(List[i], Nuum);
			Nuum++;
		}
	}
	m_CategoryNameText->SetText(FText::FromString(FString::Printf(TEXT("All"))));
	m_BlueprintsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_CraftingList->GetNumItems())));
	m_PreIndex = 0;
	/*
	if (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		Item->SetColor(FLinearColor(0.f, 0.f, 1.0f, 1.0f));

		m_CraftingList->RegenerateAllEntries();
	}
	*/
}

void UCrafting::BtnWeapon()
{
	while (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		m_CraftingList->RemoveItem(Item);
	}
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const UDataTable* Table = GameInst->GetUIItemInfo();
	FString context;
	TArray<FUIItemTableInfo*> List;
	Table->GetAllRows<FUIItemTableInfo>(context, List);
	int32 Nuum = 0;
	for (int32 i = 0; i < List.Num(); i++)
	{
		if (List[i]->MakeMaterials.Num() > 0 && List[i]->ItemType == EItemType::Weapon)
		{
			AddItem(List[i], Nuum);
			Nuum++;
		}
	}
	m_CategoryNameText->SetText(FText::FromString(FString::Printf(TEXT("Weapon"))));
	m_BlueprintsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_CraftingList->GetNumItems())));
	m_PreIndex = 0;
	/*
	if (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		Item->SetColor(FLinearColor(0.f, 0.f, 1.0f, 1.0f));

		m_CraftingList->RegenerateAllEntries();
	}
	*/
}

void UCrafting::BtnEquipment()
{
	while (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		m_CraftingList->RemoveItem(Item);
	}
	
	
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const UDataTable* Table = GameInst->GetUIItemInfo();
	FString context;
	TArray<FUIItemTableInfo*> List;
	Table->GetAllRows<FUIItemTableInfo>(context, List);
	int32 Nuum = 0;
	for (int32 i = 0; i < List.Num(); i++)
	{
		if (List[i]->MakeMaterials.Num() > 0 && List[i]->ItemType == EItemType::Equipment)
		{
			AddItem(List[i], Nuum);
			Nuum++;
		}
	}
	m_CategoryNameText->SetText(FText::FromString(FString::Printf(TEXT("Equipment"))));
	m_BlueprintsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_CraftingList->GetNumItems())));
	m_PreIndex = 0;
	/*
	if (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		Item->SetColor(FLinearColor(0.f, 0.f, 1.0f, 1.0f));

		m_CraftingList->RegenerateAllEntries();
	}
	*/
}

void UCrafting::BtnBuildItem()
{
	while (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		m_CraftingList->RemoveItem(Item);
	}
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const UDataTable* Table = GameInst->GetUIItemInfo();
	FString context;
	TArray<FUIItemTableInfo*> List;
	Table->GetAllRows<FUIItemTableInfo>(context, List);
	int32 Nuum = 0;
	for (int32 i = 0; i < List.Num(); i++)
	{
		if (List[i]->MakeMaterials.Num() > 0 && List[i]->ItemType == EItemType::BuildItem)
		{
			AddItem(List[i], Nuum);
			Nuum++;
		}
	}
	m_CategoryNameText->SetText(FText::FromString(FString::Printf(TEXT("BuildItem"))));
	m_BlueprintsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_CraftingList->GetNumItems())));
	m_PreIndex = 0;
	/*
	if (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		Item->SetColor(FLinearColor(0.f, 0.f, 1.0f, 1.0f));

		m_CraftingList->RegenerateAllEntries();
	}*/
}

void UCrafting::BtnEat()
{
	while (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		m_CraftingList->RemoveItem(Item);
	}
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const UDataTable* Table = GameInst->GetUIItemInfo();
	FString context;
	TArray<FUIItemTableInfo*> List;
	Table->GetAllRows<FUIItemTableInfo>(context, List);
	int32 Nuum = 0;
	for (int32 i = 0; i < List.Num(); i++)
	{
		if (List[i]->MakeMaterials.Num() > 0 && List[i]->ItemType == EItemType::Eat)
		{
			AddItem(List[i], Nuum);
			Nuum++;
		}
	}
	m_CategoryNameText->SetText(FText::FromString(FString::Printf(TEXT("Eat"))));
	m_BlueprintsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_CraftingList->GetNumItems())));
	m_PreIndex = 0;
	/*
	if (m_CraftingList->GetNumItems() > 0)
	{
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(m_CraftingList->GetItemAt(0));
		Item->SetColor(FLinearColor(0.f, 0.f, 1.0f, 1.0f));

		m_CraftingList->RegenerateAllEntries();
	}
	*/
}
