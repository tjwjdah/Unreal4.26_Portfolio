// Fill out your copyright notice in the Description page of Project Settings.

#include "Sword.h"
#include "SpawnWeapon.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Player/PlayerAnim.h"
USword::USword() {

	m_Type = EWeaponType::Sword;

	//CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	//CollisionVolume->SetupAttachment(RootComponent);
	//SetRootComponent(CollisionVolume);
	//m_Mesh->SetupAttachment(RootComponent);
	//SetActorLocationAndRotation(FVector(-8.274286f, 3.027441f, -1.138027f), FRotator(-167.317169f, 153.722229f, 5.770499f));
}
void USword::Use()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASpawnWeapon* Sword = GetWorld()->SpawnActor<ASpawnWeapon>(ASpawnWeapon::StaticClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);
	
	Sword->GetSkeletalMeshComponent()->AttachToComponent(m_Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("Tool"));
	Sword->SetPlayerOwner(m_Owner);
	Sword->SetAttack(m_Attack);
	Sword->SetMesh(m_SkeletalMesh);
	Sword->SetBuildAttack(m_BuildAttack);
	Sword->SetWeaponType(m_Type);
	//Sword->SetActorLocationAndRotation(FVector(-8.274286f, 3.027441f, -1.138027f), FRotator(-167.317169f, 153.722229f, 5.770499f));
	//Bow->SetMesh(TEXT("SkeletalMesh'/Game/DynamicCombatSystem/Meshes/SK_ElvenBow.SK_ElvenBow'"));

	
	//SetActorLocation(FVector(-8.274286f, 3.027441f, -1.138027f));
	//
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), m_Attack));
	m_Owner->SetWeapon(Sword);
	m_Owner->SetWeaponOn(true);
	m_Owner->GetPlayerAnim()->SetWeaponOnOff(true);
}

void USword::SetMesh(USkeletalMesh* _Mesh)
{
	m_SkeletalMesh = _Mesh;
}
