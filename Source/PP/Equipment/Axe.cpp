// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Player/PlayerAnim.h"
AAxe::AAxe() {
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Mesh);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Type = EWeaponType::Axe;
}

void AAxe::Use()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAxe* Axe = GetWorld()->SpawnActor<AAxe>(GetClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);
	//Bow->SetMesh()
	Axe->AttachToComponent(m_Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("Hand_RTool_Axe"));
	Axe->m_Mesh->SetSkeletalMesh(m_SkeletalMesh);
	Axe->SetAttack(m_Attack);
	Axe->SetBuildAttack(m_BuildAttack);
	//Bow->SetMesh(TEXT("SkeletalMesh'/Game/DynamicCombatSystem/Meshes/SK_ElvenBow.SK_ElvenBow'"));


	m_Owner->SetWeapon(Axe);
	m_Owner->SetWeaponOn(true);
	m_Owner->GetPlayerAnim()->SetWeaponOnOff(true);
}

void AAxe::SetMesh(USkeletalMesh* _Mesh)
{
	m_SkeletalMesh = _Mesh;
}
