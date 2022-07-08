// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleProjectile.h"

// Sets default values
AParticleProjectile::AParticleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_NiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticle"));
	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(m_Body);

	m_Particle->SetupAttachment(m_Body);

	m_Movement->SetUpdatedComponent(m_Body);

	m_Movement->InitialSpeed = 1000.f;
	m_Movement->ProjectileGravityScale = 0.f;

	m_Body->SetCollisionProfileName(TEXT("EnemyAttack"));

	m_ParticleType = EParticleType::Basic;

}

// Called when the game starts or when spawned
void AParticleProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_Movement->OnProjectileStop.AddDynamic(this, &AParticleProjectile::ProjectileStop);
	
}

// Called every frame
void AParticleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParticleProjectile::ProjectileStop(const FHitResult& result)
{
	StopEvent(result);
}

void AParticleProjectile::StopEvent(const FHitResult& result)
{

}

