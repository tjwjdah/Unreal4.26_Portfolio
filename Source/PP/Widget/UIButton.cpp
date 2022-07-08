// Fill out your copyright notice in the Description page of Project Settings.


#include "UIButton.h"

void UUIButton::NativeConstruct()
{
	Super::NativeConstruct();
	m_Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn")));
}

void UUIButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
