// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Arrow.h"
#include "../Character/Player/PlayerCharacter.h"
const float ArrowDamage = 10.f;
const float ArrowSpeed = 7000.f;
ABow::ABow() {
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

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


void ABow::SetMesh(USkeletalMesh* _Mesh)
{
	m_SkeletalMesh= _Mesh;
}

void ABow::ArrowVisibility(bool _bool) {
	m_ArrowMesh->SetVisibility(_bool);
}

void ABow::ArrowShot(float _AimAlpha, FTransform _Transform) {
	m_ArrowMesh->SetVisibility(false);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AArrow* Arrow = GetWorld()->SpawnActor<AArrow>(AArrow::StaticClass(),
		_Transform.GetLocation(), _Transform.GetRotation().Rotator(),
		param);

	Arrow->SetDamage(_AimAlpha * m_Attack);
	Arrow->SetBuildDamage(_AimAlpha * m_Attack);
	Arrow->SetSpeed(_AimAlpha * ArrowSpeed);
	Arrow->SetOwner(m_Owner);
	//FVector L_ArrowSpawnDirection = _Transform.GetLocation();
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f,%f,%f"), L_ArrowSpawnDirection.X, L_ArrowSpawnDirection.Y, L_ArrowSpawnDirection.Z));


}
void ABow::BeginPlay()
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

void ABow::Use()
{
	
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABow* Bow = GetWorld()->SpawnActor<ABow>(GetClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);
	//Bow->SetMesh()
	Bow->AttachToComponent(m_Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("Bow"));
	Bow->m_Mesh->SetSkeletalMesh(m_SkeletalMesh);
	//Bow->SetMesh(TEXT("SkeletalMesh'/Game/DynamicCombatSystem/Meshes/SK_ElvenBow.SK_ElvenBow'"));
	Bow->SetAttack(m_Attack);
	Bow->SetBuildAttack(m_BuildAttack);
	Bow->SetOwner(m_Owner);
	m_Owner->SetWeapon(Bow);

	
}