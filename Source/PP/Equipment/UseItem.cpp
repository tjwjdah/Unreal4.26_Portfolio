// Fill out your copyright notice in the Description page of Project Settings.


#include "UseItem.h"
#include "../Character/Player/PlayerCharacter.h"
// Sets default values
UUseItem::UUseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	

}


void UUseItem::SetPlayerOwner(APlayerCharacter* _Owner)
{
	m_Owner = _Owner;
}
