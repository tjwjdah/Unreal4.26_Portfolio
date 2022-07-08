// Fill out your copyright notice in the Description page of Project Settings.


#include "MorigeshMeteor.h"
#include "NormalEffect.h"
#include "../Character/Monster/Morigesh.h"

AMorigeshMeteor::AMorigeshMeteor() {
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_Meteora_01.Par_Meteora_01'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	
	m_Movement->InitialSpeed = 0.f;
	m_Body->SetCollisionProfileName(TEXT("EnemyAttack2"));
	m_Body->SetBoxExtent(FVector(80.f, 80.f, 80.f));
	m_Start = 0.f;
	
}

void AMorigeshMeteor::BeginPlay()
{

	Super::BeginPlay();

}

void AMorigeshMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Start += DeltaTime;
	if (m_Start > 1.f)
	{
		m_Velocity = (m_TargetVector - GetActorLocation()).Rotation();
		//SetActorRotation(m_Velocity);
		m_Movement->Velocity = UKismetMathLibrary::CreateVectorFromYawPitch(m_Velocity.Yaw, m_Velocity.Pitch, 800.f);
	}

	
}

void AMorigeshMeteor::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);
	
	if (TEXT("BPPlayerCharacter_C") == result.Actor->GetClass()->GetName())
	{

		if (IsValid(m_Owner))
		{
			FDamageEvent	DmgEvent;

			result.GetActor()->TakeDamage(m_Attack, DmgEvent,
				m_Owner->GetController(), m_Owner);
		}

	
	}
	else {
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
				FCollisionShape::MakeSphere(150.f),
				params);
/*
#if ENABLE_DRAW_DEBUG

			FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;


			DrawDebugSphere(GetWorld(), MinionLoc,
				150.f, 20, DrawColor, false,
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


	}

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	Effect->LoadParticle(TEXT("ParticleSystem'/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_Meteora_Explode_01.Par_Meteora_Explode_01'"));

	m_Particle->DestroyComponent();
	m_DecalEffect->Destroy();
	Destroy();
}
