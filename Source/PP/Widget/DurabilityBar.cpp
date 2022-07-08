// Fill out your copyright notice in the Description page of Project Settings.


#include "DurabilityBar.h"

void UDurabilityBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_DurabilityBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DurabilityBar")));
	m_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("DurabilityText")));
}

void UDurabilityBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
