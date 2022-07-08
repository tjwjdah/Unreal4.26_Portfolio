// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable.h"
#include "Components/BoxComponent.h"
#include "Engine/CollisionProfile.h"
// Sets default values
ABuildable::ABuildable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	m_DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("BuildDestructibleMesh"));
	m_DestructibleMesh->SetVisibility(false);
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildStaticMesh"));
	SetRootComponent(m_StaticMesh);
	
	m_DestructibleMesh->SetupAttachment(RootComponent);

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	Mtrl(TEXT("Material'/Game/Materials/BuildO.BuildO'"));
	if (Mtrl.Succeeded())
		CheckSucceededMaterial = Mtrl.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	Mtrl2(TEXT("Material'/Game/Materials/BuildX.BuildX'"));
	if (Mtrl2.Succeeded())
		CheckFailMaterial = Mtrl2.Object;
	HitAmount = 0;
	BuildCheck = false;
	PreBuildCheck = true;
	ActorSize = CollisionVolume->GetScaledBoxExtent();
	m_DestroyTimeMax = 2.0f;
	Destruct = false;
	m_DestroyTime = 0.f;
	FirstCheck = false;
}

// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();
	
	ActorSize = CollisionVolume->GetScaledBoxExtent();
	CollisionVolume->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	m_StaticMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	m_DestructibleMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	
}

// Called every frame
void ABuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	

	if (Destruct)
	{
		m_DestroyTime += DeltaTime;

		if (m_DestroyTimeMax <= m_DestroyTime)
		{
			Destroy();
			Destruct = false;
		}
	}
	
	
}

void ABuildable::Build()
{
	if (BaseMaterials.Num() > 0)
	{
		for (int32 i = 0; i < BaseMaterials.Num(); i++)
		{
			m_StaticMesh->SetMaterial(i, BaseMaterials[i]);
		}
	}
		
	m_StaticMesh->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionVolume->SetCollisionProfileName(TEXT("Buildable"));
}

void ABuildable::BuildChecking(FVector Location)
{
	//
	if (PreBuildCheck != BuildCheck || !FirstCheck) {
		FirstCheck = true;
		PreBuildCheck = BuildCheck;
		if (BuildCheck) {

			m_StaticMesh->SetMaterial(0, CheckSucceededMaterial);
		}
		else
		{
			m_StaticMesh->SetMaterial(0, CheckFailMaterial);
		}
	}
}


void ABuildable::SetMesh(UDestructibleMesh* _Destructiblemesh, UStaticMesh* _Staticmesh)
{
	m_DestructibleMesh->SetDestructibleMesh(_Destructiblemesh);
	m_StaticMesh->SetStaticMesh(_Staticmesh);
	for (int32 i = 0; i < m_StaticMesh->GetMaterials().Num(); i++)
		BaseMaterials.Add(m_StaticMesh->GetMaterials()[i]);


	for (int32 i = 0; i < m_StaticMesh->GetMaterials().Num(); i++)
	{
		m_StaticMesh->SetMaterial(i, nullptr);
	}

}

void ABuildable::BuildDestroy() {
	m_DestructibleMesh->SetVisibility(true);
	m_StaticMesh->SetVisibility(false);
	m_DestructibleMesh->ApplyDamage(100.f, GetActorLocation(), FVector(0.f, 0.f, -1.f) , 100.f);
	Destruct = true;
	CollisionVolume->SetCollisionProfileName("NoCollision");
	m_StaticMesh->SetCollisionProfileName("NoCollision");

	for (int32 i = 0; i < Ceilings.Num(); i++)
	{
		Ceilings[i]->Pillardelete(this);
	}

	for (int32 i = 0; i < Pillars.Num(); i++)
	{
		Pillars[i]->Ceilingsdelete(this);
	}
}

float ABuildable::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), Damage));
	Durability -= (int32)Damage;
	if (Durability <= 0)
		BuildDestroy();
	return Damage;
}

void ABuildable::Pillardelete(ABuildable* _Pillar)
{
	for (int32 i = 0; i < Pillars.Num(); i++)
	{
		if (Pillars[i] == _Pillar)
		{
			Pillars.RemoveAt(i);
			i--;
		}
	}

	if (Pillars.Num() <=1)
	{

		BuildDestroy();

	}
}

void ABuildable::Ceilingsdelete(ABuildable* _Pillar)
{
	for (int32 i = 0; i < Ceilings.Num(); i++)
	{
		if (Ceilings[i] == _Pillar)
		{
			Ceilings.RemoveAt(i);
			i--;
		}
	}
}

void ABuildable::AddPillar(ABuildable* _Pillar)
{
	for (int32 i = 0; i < Pillars.Num(); i++)
	{
		if (Pillars[i] == _Pillar)
			return;
	}
	Pillars.Add(_Pillar);
}

void ABuildable::AddCeilings(ABuildable* _Ceiling)
{
	for (int32 i = 0; i < Ceilings.Num(); i++)
	{
		if (Ceilings[i] == _Ceiling)
			return;
	}
	Ceilings.Add(_Ceiling);
}
