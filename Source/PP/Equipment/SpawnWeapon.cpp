// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnWeapon.h"

ASpawnWeapon::ASpawnWeapon() {
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(m_Mesh);
	m_ItemType = EItemType::Weapon;
}