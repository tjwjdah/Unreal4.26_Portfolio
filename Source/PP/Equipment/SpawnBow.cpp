// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBow.h"
#include "Arrow.h"

ASpawnBow::ASpawnBow() {
	SetRootComponent(m_Mesh);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Type = EWeaponType::Bow;

	m_ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/DynamicCombatSystem/Meshes/SM_ElvenArrow.SM_ElvenArrow'"));
	if (MeshAsset.Succeeded())
		m_ArrowMesh->SetStaticMesh(MeshAsset.Object);
	m_ArrowMesh->AttachToComponent(m_Mesh, FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("Arrow"));
	m_ArrowMesh->SetVisibility(false);

	m_ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FClassFinder<UAnimInstance>	BowAnimAsset(TEXT("AnimBlueprint'/Game/Item/Weapon/WeaponAnim/ABP_ElvenBow.ABP_ElvenBow_C'"));

	if (BowAnimAsset.Succeeded())
		m_Mesh->SetAnimInstanceClass(BowAnimAsset.Class);

}
void ASpawnBow::BeginPlay()
{
	Super::BeginPlay();

	if (m_Mesh->GetAnimInstance())
	{
		//	PrintViewport(1.f, FColor::Red, "BowAnim");
	}
	else {
		//PrintViewport(1.f, FColor::Red, "BowAnimxxxx");

	}
	
}


void ASpawnBow::ArrowVisibility(bool _bool)
{
	m_ArrowMesh->SetVisibility(_bool);
}

void ASpawnBow::ArrowShot(float _AimAlpha, FTransform _Transform)
{
	m_ArrowMesh->SetVisibility(false);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AArrow* Arrow = GetWorld()->SpawnActor<AArrow>(AArrow::StaticClass(),
		_Transform.GetLocation(), _Transform.GetRotation().Rotator(),
		param);

	Arrow->SetDamage(_AimAlpha * m_Attack);
	Arrow->SetBuildDamage(_AimAlpha * m_BuildAttack);
	Arrow->SetSpeed(_AimAlpha * 7000.f);
	Arrow->SetPlayerOwner(m_Owner);
}
