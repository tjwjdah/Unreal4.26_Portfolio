// Fill out your copyright notice in the Description page of Project Settings.



#include "HotKey.h"

void UHotKey::NativeConstruct()
{
	m_Key = Cast<UTextBlock>(GetWidgetFromName(TEXT("Key")));
	m_Text = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text")));
}

void UHotKey::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UHotKey::SetKeyText(FText _Key, FText _Text)
{
	m_Key->SetText(_Key);
	m_Text->SetText(_Text);
}


