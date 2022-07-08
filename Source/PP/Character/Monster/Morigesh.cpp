// Fill out your copyright notice in the Description page of Project Settings.


#include "Morigesh.h"
#include "Zoobie.h"
#include "../Player/PlayerCharacter.h"
#include "../../Effect/NormalEffect.h"
#include "../../Effect/MorigeshDagger.h"
#include "../../Effect/MorigeshExplosion.h"
#include "../../Effect/MorigeshMeteor.h"
#include "../../Effect/MagicCircle.h"
#include "../../Effect/Shield.h"
#include "MonsterAIController.h"
#include "MorigeshAnim.h"
AMorigesh::AMorigesh()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Meshes/Morigesh.Morigesh'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Character/Monster/BpMorigeshAnim.BpMorigeshAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	m_ZoobieClass = nullptr;
	m_MonsterInfoName = TEXT("Morigesh");
	m_Phase = EPhase::Phase0;
	m_MorigeshAnim = Cast<UMorigeshAnim>(GetMesh()->GetAnimInstance());
	m_SummonsZobiesCheck = true;
	m_DissolveColor = FVector(0.0f, 0.1f, 0.f);
	m_MeteorNumber = 0;
	m_MeteorOn = false;
	m_MeteorTime = 0.f;
	m_ShieldOn = false;
}

void AMorigesh::BeginPlay()
{
	GetMesh()->CreateDynamicMaterialInstance(0);

	GetMesh()->CreateDynamicMaterialInstance(2);

	GetMesh()->CreateDynamicMaterialInstance(3);
	GetMesh()->CreateDynamicMaterialInstance(4);

	GetMesh()->CreateDynamicMaterialInstance(7);

	GetMesh()->CreateDynamicMaterialInstance(9);
	Super::BeginPlay();
	FActorSpawnParameters	param;

	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	m_Circle = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation() + FVector(0.f, 60.f, -90.f), FRotator::ZeroRotator, param);

	// 애셋을 로딩한다.
	m_Circle->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Portal/Portal_04/P_Portal_04_04.P_Portal_04_04'"));

	m_Circle->SetParticleVisible(false);
	for (int32 i = 0; i < m_SummonsZoobies.Num(); i++)
	{
		m_SummonsZoobies[i]->SetSpawnPoint(this);
		m_SummonsZoobies[i]->SetMonsterInfoName(TEXT("ZombieSummon"));
		m_SummonsZoobies[i]->SetWait(true);
		m_SummonsZoobies[i]->SetSummon(true);
	}
	
}

void AMorigesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_SummonsZobiesCheck)
	{
		for (int32 i = 0; i < m_SummonsZoobies.Num(); i++)
		{
			if (!m_SummonsZoobies[i])
				continue;
			if (m_SummonsZoobies[i]->GetHp()<=0)
			{
				m_SummonsZoobies[i] = nullptr;
			}
		}
		for (int32 i = 0; i < m_SummonsZoobies.Num(); i++)
		{
			if (m_SummonsZoobies[i])
			{
				m_SummonsZobiesCheck = true;
				break;
			}
			else {
				m_SummonsZobiesCheck = false;
				
			}
		}
	}
	if (m_MeteorOn)
	{
		m_MeteorTime +=DeltaTime;
		
		if (m_MeteorTime > 1.f)
		{
			m_MeteorTime = 0.f;
			m_MeteorNumber++;
			SummonsMeteor();
			if (m_MeteorNumber >=3)
			{
				m_MeteorOn = false;
				m_MeteorNumber = 0;
				SetState(EState::End);
			}
		}
	}

	
}

float AMorigesh::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_Phase == EPhase::Phase0 || m_Shield != nullptr)
		return 0.0f;

	if ((m_MonsterInfo.HP - DamageAmount) <= 1)
	{
		m_MonsterInfo.HP = 1;
		m_ShieldOn = true; 
		if (m_Phase == EPhase::Phase1)
			SetPhase(EPhase::Phase2);
		return 0.f;
	}


	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("Hp : %d"), m_MonsterInfo.HP));
	

	
	return Damage;
}

void AMorigesh::NormalAttack()
{
	
}

void AMorigesh::DeathEffect()
{
	FActorSpawnParameters	param;

	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ANormalEffect* DeathEffect_ =GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), FRotator::ZeroRotator, param);

	// 애셋을 로딩한다.
	DeathEffect_->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Explosion/Explosion_01/P_Explosion_04.P_Explosion_04'"));
}


void AMorigesh::SetPhase(EPhase _Phase)
{
	m_Phase = _Phase;
	m_MorigeshAnim->SetPhase(_Phase);
}

void AMorigesh::SetState(EState _State)
{
	if (!m_MorigeshAnim)
	{
		m_MorigeshAnim = Cast<UMorigeshAnim>(GetMesh()->GetAnimInstance());
	}
	m_MorigeshAnim->SetState(_State);
}

EState AMorigesh::GetState()
{
	return m_MorigeshAnim->GetState();
}

void AMorigesh::SummonsZombies()
{
	if (m_Phase != EPhase::Phase0)
		return;
	m_MonsterInfo.TraceDistance = 5000.f;
	
	m_Circle->SetParticleVisible(true);
	for (int32 i = 0; i < m_SummonsZoobies.Num(); i++)
	{
		m_SummonsZoobies[i]->Summon();
		if (m_AttackPlayer)
			m_SummonsZoobies[i]->SetAttackPlayer(m_AttackPlayer);
	}
	

	
}

void AMorigesh::SummonsZombieEnd()
{
	m_Circle->Destroy();
	m_Circle = nullptr;
}



void AMorigesh::SummonsExplosion()
{
	// Muzzle의 위치를 얻어온다.

	FCollisionQueryParams	params(NAME_None, false, this);

	FHitResult	LineResult;
	bool Collision = GetWorld()->LineTraceSingleByChannel(LineResult, GetActorLocation(),
		GetActorLocation() + GetActorUpVector() * -300.f,
		ECollisionChannel::ECC_GameTraceChannel5,
		params);

	

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;




	AMorigeshExplosion* Bullet = GetWorld()->SpawnActor<AMorigeshExplosion>(
		AMorigeshExplosion::StaticClass(), LineResult.ImpactPoint+FVector(0.f,5.f,0.f), GetActorRotation(),
		param);

	m_Explosion = Bullet;
	Bullet->SetAttack(m_MonsterInfo.Attack * m_MonsterInfo.AttackInfos[GetAttackType()].Attackdiameter);
	Bullet->SetOwner(this);
	Bullet->SetPlayer(m_AttackPlayer);

}

void AMorigesh::SummonsMeteor()
{
	//PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("SummonsMeteor")));
	FCollisionQueryParams	params(NAME_None, false, this);


	FHitResult	LineResult;
	bool Collision = GetWorld()->LineTraceSingleByChannel(LineResult, m_AttackPlayer->GetActorLocation(),
		m_AttackPlayer->GetActorLocation() + FVector(0.f, 0.f, -400.f),
		ECollisionChannel::ECC_GameTraceChannel5,
		params);



	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	FVector SummonVector = LineResult.ImpactPoint + FVector(FMath::RandRange(-350.f, 350.f), FMath::RandRange(-350.f, 350.f), 800.f);

	AMorigeshMeteor* Bullet = GetWorld()->SpawnActor<AMorigeshMeteor>(
		AMorigeshMeteor::StaticClass(), SummonVector, FRotator::ZeroRotator,
		param);


	AMagicCircle* Circle = GetWorld()->SpawnActor<AMagicCircle>(
		AMagicCircle::StaticClass(), LineResult.ImpactPoint + FVector(0.f, 0.f, -100.f), FRotator::ZeroRotator,
		param);

	Bullet->SetTargetVector(LineResult.ImpactPoint);
	Bullet->SetDecal(Circle);
	

	Bullet->SetAttack(m_MonsterInfo.Attack * m_MonsterInfo.AttackInfos[GetAttackType()].Attackdiameter);
	Bullet->SetOwner(this);
	
}

AShield* AMorigesh::GetShield()
{
	return m_Shield;
}

void AMorigesh::ShieldSummon()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AShield* Shield = GetWorld()->SpawnActor<AShield>(
		AShield::StaticClass(), GetActorLocation(), FRotator::ZeroRotator,
		param);

	Shield->SetOwenr(this);
	m_Shield = Shield;
}

void AMorigesh::DeathOn()
{
	if (m_Explosion != nullptr)
	{
		m_Explosion->Destroy();
	}
	m_AnimInstance->ChangeAnimType(EMonsterAnimType::Death);

	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

	if (MonsterController)
		MonsterController->BrainComponent->StopLogic(TEXT("Dead"));


	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Death();
}



void AMorigesh::ExplosionStart()
{
	m_Explosion->Start();
}

void AMorigesh::Explosion()
{
	m_Explosion = nullptr;
	SetState(EState::End);
}


void AMorigesh::DaggerAttack()
{
	
	// Muzzle의 위치를 얻어온다.
	FVector	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("FX_Dagger"));

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMorigeshDagger* Bullet = GetWorld()->SpawnActor<AMorigeshDagger>(
		AMorigeshDagger::StaticClass(), MuzzleLoc, GetActorRotation(),
		param);

	Bullet->SetAttack(m_MonsterInfo.Attack * m_MonsterInfo.AttackInfos[GetAttackType()].Attackdiameter);
	Bullet->SetOwner(this);
	
}

