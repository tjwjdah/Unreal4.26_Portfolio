// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "../Character/Monster/Monster.h"
#include "../Effect/Shield.h"
#include "../Character/Player/PlayerCharacter.h"

AArrow::AArrow() {
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/DynamicCombatSystem/Meshes/SM_ElvenArrow.SM_ElvenArrow'"));

	if (MeshAsset.Succeeded())
		m_Mesh->SetStaticMesh(MeshAsset.Object);
	m_On = false;

	

	m_ProjectileMovement->bRotationFollowsVelocity = 1;
	m_ProjectileMovement->InitialSpeed = 4000.f;
	
}
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	m_Mesh->SetCollisionProfileName(TEXT("PlayerAttack"));
	m_ProjectileMovement->OnProjectileStop.AddDynamic(this, &AArrow::ProjectileStop);
	//m_ProjectileMovement->SetVelocityInLocalSpace(GetActorForwardVector() * m_InitialSpeed);
	//SetLifeSpan(15.f);
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), m_InitialSpeed));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f,%f,%f"), m_ProjectileMovement->Velocity.X, m_ProjectileMovement->Velocity.Y, m_ProjectileMovement->Velocity.Z));
	//m_ProjectileMovement->ProjectileGravityScale = 0.3;
}
// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (m_On)
	{
		m_Time += DeltaTime;
		if (m_Time > 5.f)
		{
			m_On = false;
			Destroy();
		}
	}
}
void AArrow::SetSpeed(float _Speed)
{
	m_ProjectileMovement->InitialSpeed = _Speed;
	m_ProjectileMovement->Velocity = GetActorForwardVector() * m_ProjectileMovement->InitialSpeed;
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f,%f,%f"), m_ProjectileMovement->Velocity.X, m_ProjectileMovement->Velocity.Y, m_ProjectileMovement->Velocity.Z));
	SetLifeSpan(15.f);

}
void AArrow::ProjectileStop(const FHitResult& result)
{
	StopEvent(result);

}

void AArrow::StopEvent(const FHitResult& result)
{
	if (result.Actor->IsA(AMonster::StaticClass()))
	{
		
		AMonster* Monster = Cast<AMonster>(result.GetActor());

		m_Mesh->AttachToComponent(Monster->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, result.BoneName);
		//PrintViewport(10.f, FColor::Blue, result.BoneName.ToString());
		//m_Mesh->AttachToComponent(Monster->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, result.BoneName);
		m_Mesh->SetCollisionProfileName(TEXT("NoCollision"));
		FVector Location =GetActorLocation() + GetActorForwardVector() *30.f;
		SetActorLocation(Location);
		FDamageEvent	DmgEvent;
		
		Monster->TakeDamage(m_Damage, DmgEvent, m_Owner->GetController(), m_Owner);
		

	}

	if (result.Actor->IsA(AShield::StaticClass()))
	{

	result.GetComponent()->OnComponentHit.Broadcast(nullptr, nullptr, nullptr, result.ImpactNormal, result);
	}
	m_On = true;
}
