// Fill out your copyright notice in the Description page of Project Settings.

#include "Sword.h"

#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Player/PlayerAnim.h"
ASword::ASword() {
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Type = EWeaponType::Sword;

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	SetRootComponent(CollisionVolume);
	m_Mesh->SetupAttachment(RootComponent);
	//SetActorLocationAndRotation(FVector(-8.274286f, 3.027441f, -1.138027f), FRotator(-167.317169f, 153.722229f, 5.770499f));
}
void ASword::Use()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASword* Sword = GetWorld()->SpawnActor<ASword>(GetClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);
	//Bow->SetMesh()
	
	Sword->AttachToComponent(m_Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("Tool"));
	Sword->SetAttack(m_Attack);
	Sword->m_Mesh->SetSkeletalMesh(m_SkeletalMesh);
	Sword->SetBuildAttack(m_BuildAttack);
	//Bow->SetMesh(TEXT("SkeletalMesh'/Game/DynamicCombatSystem/Meshes/SK_ElvenBow.SK_ElvenBow'"));

	
	//SetActorLocation(FVector(-8.274286f, 3.027441f, -1.138027f));
	//
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), m_Attack));
	m_Owner->SetWeapon(Sword);
	m_Owner->SetWeaponOn(true);
	m_Owner->GetPlayerAnim()->SetWeaponOnOff(true);
}

void ASword::SetMesh(USkeletalMesh* _Mesh)
{
	m_SkeletalMesh = _Mesh;
}
