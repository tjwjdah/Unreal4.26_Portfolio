// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "SpawnWeapon.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Player/PlayerAnim.h"
UAxe::UAxe() {
	/*
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Mesh);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	*/
	m_Type = EWeaponType::Axe;
}

void UAxe::Use()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASpawnWeapon* Axe = GetWorld()->SpawnActor<ASpawnWeapon>(ASpawnWeapon::StaticClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);
	//Bow->SetMesh()
	Axe->AttachToComponent(m_Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("Hand_RTool_Axe"));
	Axe->SetMesh(m_SkeletalMesh);
	Axe->SetAttack(m_Attack);
	Axe->SetBuildAttack(m_BuildAttack);
	Axe->GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Axe->SetWeaponType(m_Type);
	//Bow->SetMesh(TEXT("SkeletalMesh'/Game/DynamicCombatSystem/Meshes/SK_ElvenBow.SK_ElvenBow'"));


	m_Owner->SetWeapon(Axe);
	m_Owner->SetWeaponOn(true);
	m_Owner->GetPlayerAnim()->SetWeaponOnOff(true);
}

void UAxe::SetMesh(USkeletalMesh* _Mesh)
{
	m_SkeletalMesh = _Mesh;
}
