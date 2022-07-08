// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterState.h"

void UCharacterState::NativeConstruct()
{
	Super::NativeConstruct();

	m_HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	m_EnergyBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EnergyBar")));
	m_HungerBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HungerBar")));
	m_ThirstBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ThirstBar")));
}

void UCharacterState::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
