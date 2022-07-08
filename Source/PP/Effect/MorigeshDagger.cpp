// Fill out your copyright notice in the Description page of Project Settings.


#include "MorigeshDagger.h"
#include "NormalEffect.h"
#include "../Character/Monster/Morigesh.h"

AMorigeshDagger::AMorigeshDagger() {
	PrimaryActorTick.bCanEverTick = true;
	

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonMorigesh/FX/Particles/Morigesh/Abilities/Primary/FX/P_Morigesh_Dagger_Stick.P_Morigesh_Dagger_Stick'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Particle->CustomTimeDilation = 0.1f;

	m_Movement->InitialSpeed = 1750.f;


}

void AMorigeshDagger::BeginPlay()
{
	Super::BeginPlay();
}

void AMorigeshDagger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_DestroyTime += DeltaTime;
	if (m_DestroyTime >= 3.f)
	{
		m_DestroyTime = 0.f;
		Destroy();
	}
}

void AMorigeshDagger::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	// 애셋을 로딩한다.
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonMorigesh/FX/Particles/Morigesh/Abilities/SkillshotAOE/FX/P_Morigesh_DOT_Impact.P_Morigesh_DOT_Impact'"));
	/*
	// Sound
	Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	*/

	m_Particle->DestroyComponent();

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}

	Destroy();
}
