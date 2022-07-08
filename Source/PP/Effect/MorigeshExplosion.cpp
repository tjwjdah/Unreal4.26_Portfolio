// Fill out your copyright notice in the Description page of Project Settings.


#include "MorigeshExplosion.h"
#include "NormalEffect.h"
#include "../Character/Monster/Morigesh.h"

AMorigeshExplosion::AMorigeshExplosion() {
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Portal/Portal_01/P_Portal_01_05.P_Portal_01_05'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);


	m_Movement->InitialSpeed = 0.f;
	//m_Movement->bInitialVelocityInLocalSpace = false;
	m_Start = false;
	m_ExplosionTime = 0.f;
	m_Body->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	SetActorScale3D(FVector(1.f, 0.75f, 1.f));
}

void AMorigeshExplosion::BeginPlay()
{
	Super::BeginPlay();
}

void AMorigeshExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_Player)
	{
		
		FVector PlayerLo = m_Player->GetActorLocation();
		PlayerLo += FVector(0.f, 0.f, -90.f);
		m_Velocity = (PlayerLo - GetActorLocation()).Rotation();
		
		
		m_Movement->Velocity = UKismetMathLibrary::CreateVectorFromYawPitch(m_Velocity.Yaw, m_Velocity.Pitch, m_Movement->InitialSpeed);
	}
	

	if (m_Start)
	{
		m_ExplosionTime += DeltaTime;
		if (m_ExplosionTime > 3.f)
		{
			
			Explosion();
		}
	}
}


void AMorigeshExplosion::Start()
{
	m_Start = true;
	m_Movement->InitialSpeed = 400.f;
}


void AMorigeshExplosion::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), FRotator::ZeroRotator , param);

	// 애셋을 로딩한다.
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Buff/BasicDeBuff/P_BasicDeBuff_05.P_BasicDeBuff_05'"));
	/*
	// Sound
	Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	*/
	ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), FRotator::ZeroRotator, param);
	Effect2->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Explosion/Explosion_01/P_Explosion_05.P_Explosion_05'"));
	
	m_Particle->DestroyComponent();
	AMorigesh* Mori = Cast<AMorigesh>(m_Owner);

	Mori->Explosion();
	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);


	
	}
	Destroy();
	
}

void AMorigeshExplosion::Explosion()
{

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), FRotator::ZeroRotator, param);

	// 애셋을 로딩한다.
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Buff/BasicDeBuff/P_BasicDeBuff_05.P_BasicDeBuff_05'"));
	/*
	// Sound
	Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	*/
	ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), FRotator::ZeroRotator, param);
	Effect2->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Explosion/Explosion_01/P_Explosion_05.P_Explosion_05'"));
	m_Particle->DestroyComponent();
	AMorigesh* Mori = Cast<AMorigesh>(m_Owner);

	Mori->Explosion();
	if (IsValid(m_Owner))
	{

		FVector	MinionLoc = GetActorLocation();
		FVector	Forward = GetActorForwardVector();

		FCollisionQueryParams	params(NAME_None, false, this);

		// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
		FHitResult	result;
		bool Sweep = GetWorld()->SweepSingleByChannel(result, MinionLoc,
			MinionLoc, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel7,
			FCollisionShape::MakeSphere(100.f),
			params);
/*
#if ENABLE_DRAW_DEBUG

		FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;


		DrawDebugSphere(GetWorld(), MinionLoc,
			100.f, 20, DrawColor, false,
			0.3f);

#endif
*/
		if (Sweep)
		{
			FDamageEvent	DmgEvent;

			result.GetActor()->TakeDamage(m_Attack, DmgEvent,
				m_Owner->GetController(), m_Owner);
		}
		
	}
	Destroy();
}
