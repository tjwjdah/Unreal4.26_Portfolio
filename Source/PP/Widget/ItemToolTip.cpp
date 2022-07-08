// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTip.h"
#include "UIValue.h"
void UItemToolTip::NativeConstruct()
{
	Super::NativeConstruct();
	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));

	m_ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNameText")));
	m_ItemAmount = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxAMount")));
	m_ItemUsePossible = Cast<UTextBlock>(GetWidgetFromName(TEXT("UsePossible")));
	m_ItemType = Cast<UTextBlock>(GetWidgetFromName(TEXT("Type")));
	m_ItemWeaponType = Cast<UTextBlock>(GetWidgetFromName(TEXT("WeaponType")));
	m_ItemBuildType = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuildType")));
	m_ItemDesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("Desc")));

	m_OptionArray = Cast<UVerticalBox>(GetWidgetFromName(TEXT("OptionArray")));
}

void UItemToolTip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UItemToolTip::SetItem(const FUIItemTableInfo* ItemInfo,int32 ItemAMount)
{
	m_IconImage->SetBrushFromTexture(ItemInfo->IconTexture);
	m_ItemName->SetText(FText::FromString(ItemInfo->Name));
	m_ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), ItemAMount, ItemInfo->MaxAMount)));
	if (ItemInfo->UsePossible)
		m_ItemUsePossible->SetVisibility(ESlateVisibility::Visible);
	else
		m_ItemUsePossible->SetVisibility(ESlateVisibility::Collapsed);
	switch (ItemInfo->ItemType)
	{
	case EItemType::Weapon:
	{
		m_ItemType->SetText(FText::FromString(FString::Printf(TEXT("Type : Weapon"))));
		m_ItemWeaponType->SetVisibility(ESlateVisibility::Visible);
		
		switch (ItemInfo->WeaponType)
		{
		case EWeaponType::Sword:
			m_ItemWeaponType->SetText(FText::FromString(FString::Printf(TEXT("WeaponType : Sword"))));
			break;
		case EWeaponType::Bow:
			m_ItemWeaponType->SetText(FText::FromString(FString::Printf(TEXT("WeaponType : Bow"))));
			break;
		case EWeaponType::Pick:
			m_ItemWeaponType->SetText(FText::FromString(FString::Printf(TEXT("WeaponType : Pick"))));
			break;
		case EWeaponType::Axe:
			m_ItemWeaponType->SetText(FText::FromString(FString::Printf(TEXT("WeaponType : Axe"))));
			break;
		}
		m_ItemBuildType->SetVisibility(ESlateVisibility::Collapsed);
	}
		break;
	case EItemType::Equipment:
	{
		m_ItemType->SetText(FText::FromString(FString::Printf(TEXT("Type : Equipment"))));
		m_ItemWeaponType->SetVisibility(ESlateVisibility::Collapsed);
		m_ItemBuildType->SetVisibility(ESlateVisibility::Collapsed);
	}
		break;
	case EItemType::BuildItem:
	{
		m_ItemType->SetText(FText::FromString(FString::Printf(TEXT("Type : BuildItem"))));
		m_ItemWeaponType->SetVisibility(ESlateVisibility::Collapsed);
		m_ItemBuildType->SetVisibility(ESlateVisibility::Visible);

		switch (ItemInfo->EBuildType)
		{
		case EBuildType::Foundation:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Foundation"))));
			break;
		case EBuildType::Wall:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Wall"))));
			break;
		case EBuildType::Ceiling:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Ceiling"))));
			break;
		case EBuildType::DoorFrame:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : DoorFrame"))));
			break;
		case EBuildType::WindowFrame:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : WindowFrame"))));
			break;
		case EBuildType::Ramp:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Ramp"))));
			break;
		case EBuildType::Stairs:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Stairs"))));
			break;
		case EBuildType::Fence:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Fence"))));
			break;
		case EBuildType::Door:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Door"))));
			break;
		case EBuildType::Window:
			m_ItemBuildType->SetText(FText::FromString(FString::Printf(TEXT("BuildType : Window"))));
			break;
		}
	}
		break;
	case EItemType::Eat:
	{
		m_ItemType->SetText(FText::FromString(FString::Printf(TEXT("Type : Eat"))));
		m_ItemWeaponType->SetVisibility(ESlateVisibility::Collapsed);
		m_ItemBuildType->SetVisibility(ESlateVisibility::Collapsed);
	}
		break;
	case EItemType::Material:
	{
		m_ItemType->SetText(FText::FromString(FString::Printf(TEXT("Type : Material"))));
		m_ItemWeaponType->SetVisibility(ESlateVisibility::Collapsed);
		m_ItemBuildType->SetVisibility(ESlateVisibility::Collapsed);
	}
		break;
	}
	m_ItemDesc->SetText(FText::FromString(ItemInfo->Desc));
	m_OptionArray->ClearChildren();
	FStringClassReference MyWidgetClassRef(TEXT("WidgetBlueprint'/Game/UI/UI_Value.UI_Value_C'"));
	UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUIValue>();
	for (int32 i = 0; i < ItemInfo->OptionArray.Num(); i++)
	{
		
		UUIValue* UIValue = CreateWidget<UUIValue>(GetWorld(), MyWidgetClass);
		UIValue->AddToViewport();
		switch (ItemInfo->OptionArray[i].OptionType)
		{
		case EItemOption::Attack:
			UIValue->SetValue(FString::Printf(TEXT("Attack")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::Armor:
			UIValue->SetValue(FString::Printf(TEXT("Armor")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::HPMax:
			UIValue->SetValue(FString::Printf(TEXT("HPMax")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::HPRecovery:
			UIValue->SetValue(FString::Printf(TEXT("HPRecovery")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::EnergyRecovery:
			UIValue->SetValue(FString::Printf(TEXT("EnergyRecovery")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::HungerRecovery:
			UIValue->SetValue(FString::Printf(TEXT("HungerRecovery")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::ThirstRecovery:
			UIValue->SetValue(FString::Printf(TEXT("ThirstRecovery")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::Critical:
			UIValue->SetValue(FString::Printf(TEXT("Critical")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::Durability:
			UIValue->SetValue(FString::Printf(TEXT("Durability")), ItemInfo->OptionArray[i].Option);
			break;
		case EItemOption::BuildAttack:
			UIValue->SetValue(FString::Printf(TEXT("Build Attack")), ItemInfo->OptionArray[i].Option);
			break;
		}
	
		m_OptionArray->AddChild(UIValue);

	}
}
