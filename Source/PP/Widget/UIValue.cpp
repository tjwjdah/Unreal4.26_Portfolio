// Fill out your copyright notice in the Description page of Project Settings.


#include "UIValue.h"

void UUIValue::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemOption = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_ItemValue = Cast<UTextBlock>(GetWidgetFromName(TEXT("Value")));
}

void UUIValue::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UUIValue::SetValue(FString _Name, int32 _Value)
{
	m_ItemOption->SetText(FText::FromString(_Name));
	m_ItemValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), _Value)));
}
