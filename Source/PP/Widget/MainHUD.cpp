// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();
	m_Crosshair = Cast<UCrosshair>(GetWidgetFromName(TEXT("UI_Crosshair")));
	m_Inventory = Cast<UInventoryTile>(GetWidgetFromName(TEXT("UI_Inventory")));
	m_UseInventory = Cast<UUseInventoryTile>(GetWidgetFromName(TEXT("UI_UseItemInventory")));
	m_UseGarmentInventory = Cast<UUseGarmentInventoryTile>(GetWidgetFromName(TEXT("UI_UseGarmentInventory")));
	m_UIOn = Cast<UBorder>(GetWidgetFromName(TEXT("UI_On")));
	m_Loot = Cast<UItemLoot>(GetWidgetFromName(TEXT("UI_Loot")));
	m_HotKey =Cast<UHotKey>(GetWidgetFromName(TEXT("UI_HotKey")));
	m_CharacterState = Cast<UCharacterState>(GetWidgetFromName(TEXT("UI_CharacterState")));
	m_DurabilityBar = Cast<UDurabilityBar>(GetWidgetFromName(TEXT("UI_DurabilityBar")));
	m_Crafting = Cast<UCrafting>(GetWidgetFromName(TEXT("UI_Crafting")));
	
	m_QuestWidget = Cast<UQuestWidget>(GetWidgetFromName(TEXT("UI_Quest")));
	m_SelectedBlueprintInfo = Cast<USelectedBlueprintInfo>(GetWidgetFromName(TEXT("UI_SelectedBlueprintInfo")));
	int32 a = 0;

}
void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//m_Crosshair->SetCrosshairPosition(FVector(100.f, 100.f, 100.f));
	
}

void UMainHUD::UI_OnOff(bool _On)
{
	if (_On)
	{
		m_UIOn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		APlayerController* pController = GetWorld()->GetFirstPlayerController();
		pController->SetInputMode(FInputModeGameAndUI());
		pController->bShowMouseCursor = true;
	}
	else
	{
		m_UIOn->SetVisibility(ESlateVisibility::Collapsed);
		APlayerController* pController = GetWorld()->GetFirstPlayerController();
		pController->SetInputMode(FInputModeGameOnly());
		pController->bShowMouseCursor = false;
	}
		
	
}

