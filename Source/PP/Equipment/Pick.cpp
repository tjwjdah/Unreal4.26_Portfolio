// Fill out your copyright notice in the Description page of Project Settings.


#include "Pick.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Player/PlayerAnim.h"
APick::APick() {
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Mesh);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Type = EWeaponType::Pick;
}

void APick::Use()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APick* Pick = GetWorld()->SpawnActor<APick>(GetClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);
	//Bow->SetMesh()
	Pick->AttachToComponent(m_Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("Hand_RTool_Pick"));
	Pick->m_Mesh->SetSkeletalMesh(m_SkeletalMesh);
	Pick->SetAttack(m_Attack);
	//Bow->SetMesh(TEXT("SkeletalMesh'/Game/DynamicCombatSystem/Meshes/SK_ElvenBow.SK_ElvenBow'"));

	Pick->SetBuildAttack(m_BuildAttack);
	m_Owner->SetWeapon(Pick);
	m_Owner->SetWeaponOn(true);
	m_Owner->GetPlayerAnim()->SetWeaponOnOff(true);
}

void APick::SetMesh(USkeletalMesh* _Mesh)
{
	m_SkeletalMesh = _Mesh;
}
