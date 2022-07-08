// Fill out your copyright notice in the Description page of Project Settings.


#include "EatItem.h"
#include "../Character/Player/PlayerCharacter.h"
AEatItem::AEatItem() {
	m_ItemType = EItemType::Eat;
}

void AEatItem::BeginPlay()
{
}

void AEatItem::Tick(float DeltaTime)
{
}

void AEatItem::Use()
{
	m_Owner->SetEatItemOn(true);
}
