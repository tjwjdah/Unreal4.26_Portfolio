// Fill out your copyright notice in the Description page of Project Settings.


#include "EatItem.h"
#include "../Character/Player/PlayerCharacter.h"
UEatItem::UEatItem() {
	m_ItemType = EItemType::Eat;
}



void UEatItem::Use()
{
	m_Owner->SetEatItemOn(true);
}
