// Fill out your copyright notice in the Description page of Project Settings.


#include "UseBuildable.h"
#include "../Buildable/BuildManagerComponent.h"
#include "../Character/Player/PlayerCharacter.h"
AUseBuildable::AUseBuildable() {
	m_ItemType = EItemType::BuildItem;
}


void AUseBuildable::Use() {
	m_Owner->GetBuildManager()->SetMesh(m_DestructibleMesh, m_StaticMesh);
	m_Owner->GetBuildManager()->SetDurability(Durability);
	m_Owner->GetBuildManager()->SetMaxDurability(MaxDurability);
	m_Owner->GetBuildManager()->SetBuildType(m_BuildType);
	m_Owner->GetBuildManager()->SetMaterialType(m_MaterialType);
}