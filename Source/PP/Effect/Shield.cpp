// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "NiagaraFunctionLibrary.h"
#include "../Character/Monster/Morigesh.h"
#include "NormalEffect.h"
// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	m_PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	m_NiagaraParticle = CreateDefaultSubobject<UNiagaraSystem>(TEXT("NiagaraParticle"));
	SetRootComponent(m_StaticMesh);
	m_PointLight->SetupAttachment(m_StaticMesh);


	static ConstructorHelpers::FObjectFinder<UStaticMesh>	ParticleAsset(TEXT("StaticMesh'/Game/Shield_FX_Functional/Resource/mesh/SM_Sphere_01.SM_Sphere_01'"));
	if (ParticleAsset.Succeeded())
		m_StaticMesh->SetStaticMesh(ParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraParticle(TEXT("NiagaraSystem'/Game/Shield_FX_Functional/Hit_Particles/NS_Hit_Electric_Dark.NS_Hit_Electric_Dark'"));
	if (NiagaraParticle.Succeeded())
		m_NiagaraParticle= NiagaraParticle.Object;

	m_PointLight->SetIndirectLightingIntensity(1000.f);
	m_PointLight->LightColor = FColor(1.f, 0.f, 0.f);
	m_PointLight->AttenuationRadius = 1000.f;

	m_StaticMesh->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));

	m_StaticMesh->OnComponentHit.AddDynamic(this, &AShield::OnHit);
	m_Hp = 5;

	m_StaticMesh->SetCollisionProfileName(TEXT("Shield"));
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Owner != nullptr)
	{
		SetActorLocation(m_Owner->GetActorLocation());
	}
}

void AShield::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComponent != nullptr)
		return;
	m_Hp--;
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("asdsadsadsadsad")));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_NiagaraParticle, Hit.ImpactPoint, UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal),FVector(0.5f,0.5f,0.5f));

	if (m_Hp == 0)
	{

		AMorigesh* Monster = Cast<AMorigesh>(m_Owner);
		if (Monster)
			Monster->DeathOn();
		FActorSpawnParameters	param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ANormalEffect* m_Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation() + FVector(0.f, 0.f, -80.f), FRotator::ZeroRotator, param);

		// 애셋을 로딩한다.
		m_Effect->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Explosion/Explosion_01/P_Explosion_01.P_Explosion_01'"));
		m_Effect->SetActorScale3D(FVector(2.f, 2.f, 2.f));
		Destroy();
	}
}

