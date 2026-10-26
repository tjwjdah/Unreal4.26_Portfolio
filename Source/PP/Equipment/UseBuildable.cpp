// Fill out your copyright notice in the Description page of Project Settings.


#include "UseBuildable.h"
#include "../PPGameInstance.h"
#include "../Buildable/BuildManagerComponent.h"
#include "../Character/Player/PlayerCharacter.h"
UUseBuildable::UUseBuildable() {
	m_ItemType = EItemType::BuildItem;
}


void UUseBuildable::Use() {
	if (BuildDefinitionName.IsEmpty())return;
	
	m_Owner->GetBuildManager()->BuilingDestroy();
	m_Owner->GetBuildManager()->SetBuildData(BuildDefinitionName);

}